#include "NetworkInterfaceManager/UDPReceiver.hpp"


namespace sensormoniteringhub{
    namespace networkinterfacemanager{
        /// @brief starting the service and start listening and receiving udp data
        void UDPReceiver::StartService()
        {
            auto SharedDataStoreInstance_{
                std::dynamic_pointer_cast<systemcontext::SharedDataStore>( systemcontext::ComponentRegistry::GetComponent("SharedDataStore"))
            };
            if(SharedDataStoreInstance_ != nullptr){
                setPortNumber(SharedDataStoreInstance_->GetUdpPortNumber());
                setTimeoutSeconds(SharedDataStoreInstance_->GetUdpTimeOut());
                udpThreadStopSignal_.store(false);
                bool isSocketCreated{
                    CreateAndBindSocket()
                };
                if(!isSocketCreated){
                    logger::Logger::LOG("UDPReceiver::StartService", "Socket creation and binding failed!", logger::LOGLEVEL::ERROR_LEVEL);    
                    return;
                }
                logger::Logger::LOG("UDPReceiver::StartService", "Socket creation and binding successful!");    
                udpReceiverThread_ = std::thread(&UDPReceiver::UdpReceiverLoop, this);
                udpReceivedDataHandleToSdrThread_ = std::thread(&UDPReceiver::UdpReceivedDataHandlerToSdrLoop, this);
            }else{
                logger::Logger::LOG("UDPReceiver::StartService", "Shared Data Store Instance is not available!", logger::LOGLEVEL::ERROR_LEVEL);
            }
        }

        /// @brief helper method for creating and binding socket
        /// @return true if successful bind and creation, false otherwise
        bool UDPReceiver::CreateAndBindSocket(){
            sock_ = socket(AF_INET, SOCK_DGRAM, 0);
            if(sock_ < 0){
                int saved_errno = errno;
                std::string error_msg = std::string("Socket creation failed: ") + strerror(saved_errno);
                logger::Logger::LOG("UDPReceiver::CreateAndBindSocket", error_msg, logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            // for making socket non-blocking
            fcntl(sock_, F_SETFL, O_NONBLOCK);
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(portNumber_);
            addr.sin_addr.s_addr = INADDR_ANY;
            if(bind(sock_, (sockaddr*)&addr, sizeof(addr)) < 0){
                int saved_errno = errno;
                std::string error_msg = std::string("bind failed: ") + strerror(saved_errno);
                logger::Logger::LOG("UDPReceiver::CreateAndBindSocket", error_msg, logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            return true;
        }

        /// @brief method for running in loop to receive the udp data
        void UDPReceiver::UdpReceiverLoop(){
            auto TimerServiceInstance_{
                std::dynamic_pointer_cast<timerservice::TimerService>(systemcontext::ComponentRegistry::GetComponent("TimerService"))
            };
            if(TimerServiceInstance_ == nullptr){
                logger::Logger::LOG("UDPReceiver::UdpReceiverLoop", "TimerService Instance not available", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            lastDataReceivedTimeStamp_ = TimerServiceInstance_->GetCurrentTimestamp();
            while(!udpThreadStopSignal_.load()){
                char buffer[4096];
                sockaddr_in sender{};
                socklen_t senderLen = sizeof(sender);
                ssize_t bytes = recvfrom(sock_, buffer, sizeof(buffer), 0,
                                        (sockaddr*)&sender, &senderLen);
                if(bytes < 0){
                    if(!hasActiveSender_){
                        continue;
                    }
                    if (errno == EWOULDBLOCK || errno == EAGAIN){
                        // timer logic
                        if(TimerServiceInstance_->GetElapsedSeconds(lastDataReceivedTimeStamp_) >= timeOutSeconds_){
                            hasActiveSender_ = false;
                            logger::Logger::LOG("UDPReceiver::UdpReceiverLoop", "Timeout Elapsed. Looking for the next sender");
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }else{
                        int saved_errno = errno;
                        std::string error_msg = std::string("recvfrom: ") + strerror(saved_errno);
                        logger::Logger::LOG("UDPReceiver::UdpReceiverLoop", error_msg, logger::LOGLEVEL::ERROR_LEVEL);
                        continue;
                    }
                }
                // resetting the received time
                lastDataReceivedTimeStamp_ = TimerServiceInstance_->GetCurrentTimestamp();
                std::string msg(buffer, buffer + bytes);
                if(!hasActiveSender_){
                    if (IsHandshake(msg)){
                        activeSender_ = sender;
                        hasActiveSender_ = true;
                        SendReply(sender, "READY");
                        std::string senderAddressStr{inet_ntoa(sender.sin_addr)};
                        logger::Logger::LOG("UDPReceiver::UdpReceiverLoop", "Active sender found; Sender [address:port | "+senderAddressStr +" : " +std::to_string(sender.sin_port)+"]");
                    }
                    continue;
                }
                if(hasActiveSender_ && (msg.size() == 3)){
                    if(msg == std::string_view{"BYE"}){
                        hasActiveSender_ = false;
                        logger::Logger::LOG("UDPReceiver::UdpReceiverLoop", "UDP sender completed sending data. Looking for the next sender");
                        continue;
                    }
                }
                if (!IsSameSender(sender)){
                    SendReply(sender, "BUSY");
                    continue;
                }
                // converting the received data to byte vector and storing it in queue for sending with SensorDataReceiver
                std::vector<uint8_t> data(buffer, buffer + bytes);
                {
                    std::lock_guard<std::mutex> lock(receivedDataBufferQueueMutex_);
                    receivedDataBufferQueue_.push(data);
                }
                dataReceivedNotifierCv_.notify_one();
            }
        }

        /// @brief helper method to validate the handshake message
        /// @param msg 
        /// @return 
        bool UDPReceiver::IsHandshake(const std::string& msg)
        {
            bool result{false};
            if(msg.rfind("Hello from", 0) == 0){
                std::istringstream iss(msg);
                std::vector<std::string> words(
                    std::istream_iterator<std::string>{iss},
                    std::istream_iterator<std::string>{}
                );
                /**
                 * If the words sent for handshake 3 or more
                 * the ECU client name will be the third word
                 * Storing the currentECU name
                 */
                if(words.size() > 2){
                    currentEcuName = words[2];
                    result = true;
                }
            }
            return result;
        }

        /// @brief helper method to send the reply back to the sender
        /// @param sender 
        /// @param msg 
        void UDPReceiver::SendReply(const sockaddr_in& sender, const std::string& msg)
        {
            sendto(sock_, msg.c_str(), msg.size(), 0, (sockaddr*)&sender, sizeof(sender));
            std::string senderAddressStr{inet_ntoa(sender.sin_addr)};
            logger::Logger::LOG("UDPReceiver::SendReply", "Reply sent to sender [address:port | "+ senderAddressStr+" : " +std::to_string(sender.sin_port)+"] : Reply Message: "+msg);
        }

        /// @brief helper method to check whether we received data from the same sender or a different sender
        /// @param sender 
        /// @return 
        bool UDPReceiver::IsSameSender(const sockaddr_in& sender)
        {
            return sender.sin_addr.s_addr == activeSender_.sin_addr.s_addr &&
                sender.sin_port == activeSender_.sin_port;
        }

        /// @brief method to run in separate thread and waits for udp data to be pushed into the queue, then send it to sensordatareceiver
        void UDPReceiver::UdpReceivedDataHandlerToSdrLoop(){
            auto SensorDataReceiverInstance_{
                std::dynamic_pointer_cast<sensordatareceiver::SensorDataReceiver>(
                    systemcontext::ComponentRegistry::GetComponent("SensorDataReceiver")
                )
            };
            if(SensorDataReceiverInstance_ == nullptr){
                logger::Logger::LOG("UDPReceiver::UdpReceivedDataHandlerToSdrLoop", "SensorDataReceiver instance is not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            while(!udpThreadStopSignal_.load()){
                std::vector<uint8_t> data;
                {
                    std::unique_lock<std::mutex> lock(receivedDataBufferQueueMutex_);
                    dataReceivedNotifierCv_.wait(lock, [this]{
                        return udpThreadStopSignal_.load() || !receivedDataBufferQueue_.empty();
                    });
                    if(udpThreadStopSignal_.load()){
                        return;
                    }
                    data = receivedDataBufferQueue_.front();
                    receivedDataBufferQueue_.pop();
                }
                logger::Logger::LOG("UDPReceiver::UdpReceivedDataHandlerToSdrLoop", "Sending raw data to SensorDataReceiver for processing.", logger::LOGLEVEL::DEBUG_LEVEL);
                SensorDataReceiverInstance_->ProcessReceivedUdpData(data);
            }
        }

        /// @brief for stop receiving the udp data
        void UDPReceiver::StopService()
        {
            udpThreadStopSignal_.store(true);
            dataReceivedNotifierCv_.notify_one();
            if(udpReceiverThread_.joinable()){
                udpReceiverThread_.join();
            }
            if(udpReceivedDataHandleToSdrThread_.joinable()){
                udpReceivedDataHandleToSdrThread_.join();
            }
            if (sock_ >= 0){
                close(sock_);
            }
            ClearData();
            logger::Logger::LOG("UDPReceiver::StopService", "Joined UDP receiver thread!");
        }
        /// @brief method for clearing the config data members before stopping service
        void UDPReceiver::ClearData(){
            logger::Logger::LOG("UDPReceiver::ClearData", "Clearing UDP receiver data members!");
            portNumber_ = 0U;
            timeOutSeconds_ = 0U;
            currentEcuName = "";
            sock_ = -1;
            receivedDataBufferQueue_;
            activeSender_ = {};
            hasActiveSender_ = false;
            lastDataReceivedTimeStamp_ = 0U;
        }
        void UDPReceiver::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("UDPReceiver", std::make_shared<UDPReceiver>());
            logger::Logger::LOG("UDPReceiver::Initialize", "Initialization successful!");
        }

        /// @brief finalise method
        void UDPReceiver::Finalize()
        {
        }

        /// @brief setter method for port number
        /// @param portNumber 
        void UDPReceiver::setPortNumber(uint16_t portNumber){
            portNumber_ = portNumber;
        }
        
        /// @brief setter method for timeout
        /// @param seconds 
        void UDPReceiver::setTimeoutSeconds(uint16_t seconds){
            timeOutSeconds_ = seconds;
        }

        /// @brief getter method for getting the current sensor ecu status
        /// @return true if active sensor is present and name of the current udp sensor ecu which is the last connected one
        std::pair<bool, std::string> UDPReceiver::GetSensorEcuStatus(){
            return {hasActiveSender_, currentEcuName};
        }
    }
}

