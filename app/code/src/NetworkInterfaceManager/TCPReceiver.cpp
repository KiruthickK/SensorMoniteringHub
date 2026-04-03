#include <NetworkInterfaceManager/TCPReceiver.hpp>

namespace sensormoniteringhub{
    namespace networkinterfacemanager{
        /// @brief method for starting service
        void TCPReceiver::StartService()
        {
            auto sharedDataReceiverInstance{
                std::dynamic_pointer_cast<systemcontext::SharedDataStore>(
                    systemcontext::ComponentRegistry::GetComponent("SharedDataStore")
                )
            };
            if(!sharedDataReceiverInstance){
                logger::Logger::LOG("TCPReceiver::StartService", "Shared Data Receiver Instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            controlClientPortNumber_ = sharedDataReceiverInstance->GetTcpPortNumber();
            controlClientTimeOutSeconds_ = sharedDataReceiverInstance->GetTcpTimeOut();
            tcpControlClientReceiverStopSignal_.store(false);
            tcpControlClientReceiverThread_ = std::thread(&TCPReceiver::TcpReceiverLoopForControlClient, this);
        }

        /// @brief method for stopping service
        void TCPReceiver::StopService()
        {
            // Joining ClientRequestService thread
            tcpClientRequestServiceReceiverStopSignal_.store(true);
            if (tcpClientReceiverServiceSocket_ != -1) {
                shutdown(tcpClientReceiverServiceSocket_, SHUT_RDWR);
                close(tcpClientReceiverServiceSocket_);
            }
            tcpControlClientReceiverStopSignal_.store(true);
            if(tcpControlClientSocket_ != -1){
                shutdown(tcpControlClientSocket_, SHUT_RDWR);
                close(tcpControlClientSocket_);
            }
        }

        /// @brief method for creating instance of the class and registering it
        void TCPReceiver::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("TCPReceiver", std::make_shared<TCPReceiver>());
            logger::Logger::LOG("TCPReceiver::Initialize", "Initialization successful!");
        }

        void TCPReceiver::Finalize()
        {
        }

        /// @brief method to run in thread for receiving orders from tcp order client
        void TCPReceiver::TcpReceiverLoopForControlClient(){
            tcpControlClientSocket_ = CreateAndBindTcpSocket(controlClientPortNumber_);
            if(tcpControlClientSocket_ == -1){
                logger::Logger::LOG("TCPReceiver::TcpReceiverLoopForControlClient", "Socket creation and Bind failed", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            while(!tcpControlClientReceiverStopSignal_.load()){
                sockaddr_in clientAddr{};
                socklen_t clientLen = sizeof(clientAddr);
                int clientSocket = accept(tcpControlClientSocket_, (sockaddr*)&clientAddr, &clientLen);
                if (clientSocket < 0)
                {
                    if (tcpControlClientReceiverStopSignal_.load()) //received shutdown signal from stopservice
                    {
                        break;
                    }
                    logger::Logger::LOG("TCPReceiver::TcpReceiverLoopForControlClient", "Accept failed", logger::LOGLEVEL::ERROR_LEVEL);
                    continue;
                }
                std::string clientAddressStr{inet_ntoa(clientAddr.sin_addr)};
                logger::Logger::LOG("TCPReceiver::TcpReceiverLoopForControlClient", "Client connected. [Client Address : " + clientAddressStr+"][Client port : "+ std::to_string(clientAddr.sin_port)+"]");
                HandleCurrentControlClient(clientSocket);
            }
        }

        /// @brief method for creating and binding socket
        /// @param portNumber 
        /// @return 
        int TCPReceiver::CreateAndBindTcpSocket(uint16_t const portNumber){
            int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //clientRequestServiceSock_
            if (serverSocket < 0)
            {
                logger::Logger::LOG("TCPReceiver::CreateAndBindTcpSocket", "Socket creation failed", logger::LOGLEVEL::ERROR_LEVEL);
                return -1;
            }
            int opt = 1;
            setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            sockaddr_in serverAddr{};
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(portNumber);
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
            {
                logger::Logger::LOG("TCPReceiver::CreateAndBindTcpSocket", "Bind failed", logger::LOGLEVEL::ERROR_LEVEL);
                close(serverSocket);
                return -1;
            }
            if (listen(serverSocket, 5) < 0)
            {
                logger::Logger::LOG("TCPReceiver::CreateAndBindTcpSocket", "Listen failed", logger::LOGLEVEL::ERROR_LEVEL);
                return -1;
            }
            logger::Logger::LOG("TCPReceiver::CreateAndBindTcpSocket", "Socket creation and binding successful!");
            logger::Logger::LOG("TCPReceiver::CreateAndBindTcpSocket", "Server listening on port: " + std::to_string(portNumber));
            return serverSocket;
        }

        /// @brief method for starting receiving client request service thread
        /// @param clientRequestServiceSock 
        /// @param timeOut 
        void TCPReceiver::StartReceivingClientRequestServiceThread(int const clientRequestServiceSock, uint16_t const timeOut){
            logger::Logger::LOG("TCPReceiver::StartReceivingClientRequestServiceThread","Creating thread for receiving requests from TCP client");
            tcpClientRequestServiceReceiverStopSignal_.store(false);
            tcpClientRequestServiceReceiverThread_ = std::thread(&TCPReceiver::TcpReceiverLoopForClientRequestService, this, clientRequestServiceSock, timeOut);
        }

        /// @brief method for looping client request service
        /// @param clientRequestServiceSock 
        /// @param timeOut 
        void TCPReceiver::TcpReceiverLoopForClientRequestService(int const clientRequestServiceSock, uint16_t const timeOut){
            tcpClientReceiverServiceSocket_ = clientRequestServiceSock;
            while(!tcpClientRequestServiceReceiverStopSignal_.load()){
                sockaddr_in clientAddr{};
                socklen_t clientLen = sizeof(clientAddr);
                int clientSocket = accept(tcpClientReceiverServiceSocket_, (sockaddr*)&clientAddr, &clientLen);
                if (clientSocket < 0)
                {
                    if (tcpClientRequestServiceReceiverStopSignal_.load()) //received shutdown signal from stopservice
                    {
                        break;
                    }
                    logger::Logger::LOG("TCPReceiver::TcpReceiverLoopForClientRequestService", "Accept failed", logger::LOGLEVEL::ERROR_LEVEL);
                    continue;
                }
                std::string clientAddressStr{inet_ntoa(clientAddr.sin_addr)};
                logger::Logger::LOG("TCPReceiver::TcpReceiverLoopForClientRequestService", "Client connected. [Client Address : " + clientAddressStr+"][Client port : "+ std::to_string(clientAddr.sin_port)+"]");
                HandleCurrentRequestClient(clientSocket);
            }
            // thread joining
        }

        /// @brief Method for Handling current control client
        /// @param clientSocket 
        void TCPReceiver::HandleCurrentControlClient(int clientSocket){
            /**
             * @todo
             */
            std::string buffer;
            char recvBuffer[4096];
            timeval tv{};
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            int idleSeconds = 0; /** @todo */
            auto controlCommandServiceInstance{std::dynamic_pointer_cast<controlcommandservice::ControlCommandService>(
                systemcontext::ComponentRegistry::GetComponent("ControlCommandService")
            )};
            if(!controlCommandServiceInstance){
                logger::Logger::LOG("TCPReceiver::HandleCurrentControlClient", "Control Command Service Instance not available", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            while(!tcpControlClientReceiverStopSignal_.load()){
                int bytes = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
                // case when we received a command from the control client
                if (bytes > 0){
                    idleSeconds = 0;
                    buffer.append(recvBuffer, bytes);
                    size_t pos;
                    while ((pos = buffer.find('\n')) != std::string::npos){
                        std::string command = buffer.substr(0, pos);
                        buffer.erase(0, pos + 1);
                        logger::Logger::LOG("TCPReceiver::HandleCurrentControlClient", "Received command from control client");
                        logger::Logger::LOG("TCPReceiver::HandleCurrentControlClient", "Received command string: " + command, logger::LOGLEVEL::DEBUG_LEVEL);
                        std::string response;
                        response = controlCommandServiceInstance->HandleControlCommand(command);
                        response += "\n";
                        size_t totalSent = 0;
                        while (totalSent < response.size())
                        {
                            ssize_t sent = send(
                                clientSocket, response.c_str() + totalSent, response.size() - totalSent,
                                MSG_NOSIGNAL
                            );
                            if (sent <= 0)
                            {
                                logger::Logger::LOG("TCPReceiver::HandleCurrentControlClient", "Send failed", logger::LOGLEVEL::ERROR_LEVEL);
                                break;
                            }
                            totalSent += sent;
                        }
                    }
                }
                // case when the control client disconnected
                else if (bytes == 0){
                    logger::Logger::LOG("TCPReceiver::HandleCurrentControlClient", "Control client disconnected");
                    break;
                }
                // case when there's no command received within 1 sec interval
                else{
                    if (errno == EWOULDBLOCK || errno == EAGAIN)
                    {
                        idleSeconds++;
                        if (idleSeconds >= 15)
                        {
                            logger::Logger::LOG("TCPReceiver::HandleCurrentControlClient", "Control client idle timeout", logger::LOGLEVEL::ERROR_LEVEL);
                            break;
                        }
                        continue;
                    }
                    else
                    {
                        logger::Logger::LOG("TCPReceiver::HandleCurrentControlClient", "Recv error", logger::LOGLEVEL::ERROR_LEVEL);
                        break;
                    }
                }
            }
            logger::Logger::LOG("TCPReceiver::HandleCurrentControlClient", "Closing control client socket");
            close(clientSocket);
        }

        /// @brief method for handling current request client
        /// @param clientSocket 
        void TCPReceiver::HandleCurrentRequestClient(int clientSocket){
            std::string buffer;
            char recvBuffer[4096];
            // Set recv timeout to 1 second
            timeval tv{};
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            int idleSeconds = 0; // @todo
            auto clientRequestServiceInstance{std::dynamic_pointer_cast<clientrequestservice::ClientRequestService>(
                systemcontext::ComponentRegistry::GetComponent("ClientRequestService")
            )};
            if(!clientRequestServiceInstance){
                logger::Logger::LOG("TCPReceiver::HandleCurrentRequestClient","Client Request Service Instance not available",logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            while (!tcpClientRequestServiceReceiverStopSignal_.load())
            {
                int bytes = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
                // case when we received request from the active client
                if (bytes > 0)
                {
                    idleSeconds = 0; // reset idle timer @todo
                    buffer.append(recvBuffer, bytes);
                    size_t pos;
                    while ((pos = buffer.find('\n')) != std::string::npos)
                    {
                        std::string request = buffer.substr(0, pos);
                        buffer.erase(0, pos + 1);
                        logger::Logger::LOG("TCPReceiver::HandleCurrentRequestClient","Received Request from client");
                        logger::Logger::LOG("TCPReceiver::HandleCurrentRequestClient","Received Request string: "+request, logger::LOGLEVEL::DEBUG_LEVEL);
                        std::string response;
                        response = clientRequestServiceInstance->HandleRequest(request);
                        response += "\n";
                        // sending response // @todo
                        size_t totalSent = 0;
                        while (totalSent < response.size())
                        {
                            ssize_t sent = send(
                                clientSocket, response.c_str() + totalSent, response.size() - totalSent,
                                MSG_NOSIGNAL
                            );
                            if (sent <= 0)
                            {
                                logger::Logger::LOG("TCPReceiver::HandleCurrentRequestClient","Send failed", logger::LOGLEVEL::ERROR_LEVEL);
                                break;
                            }
                            totalSent += sent;
                        }
                    }
                }
                // case when the current client disconnected
                else if (bytes == 0)
                {
                    logger::Logger::LOG("TCPReceiver::HandleCurrentRequestClient","Client disconnected");
                    break;
                }
                // case when there's no request received with 1 sec interval
                else
                {
                    // timeout or error
                    if (errno == EWOULDBLOCK || errno == EAGAIN)
                    {
                        // @todo the next block
                        {
                            idleSeconds++;
                            if (idleSeconds >= 15){
                                logger::Logger::LOG("TCPReceiver::HandleCurrentRequestClient","Client idle timeout", logger::LOGLEVEL::WARNING_LEVEL);
                                break;
                            }
                        }

                        continue; // try again
                    }
                    else
                    {
                        logger::Logger::LOG("TCPReceiver::HandleCurrentRequestClient","Recv error", logger::LOGLEVEL::ERROR_LEVEL);
                        break;
                    }
                }
            }
            logger::Logger::LOG("TCPReceiver::HandleCurrentRequestClient","Closing client socket");
            close(clientSocket);
        }
    }
}

