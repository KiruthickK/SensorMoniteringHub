#include "SystemContext/SharedDataStore.hpp"

namespace sensormoniteringhub{
    namespace systemcontext{
        void SharedDataStore::StartService()
        {
        }

        void SharedDataStore::StopService()
        {
        }

        void SharedDataStore::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("SharedDataStore", std::make_shared<SharedDataStore>());
            logger::Logger::LOG("SharedDataStore::Initialize", "Initialization successful!");
        }

        void SharedDataStore::Finalize()
        {
        }

        /// @brief Sets the maximum event limit for the SharedDataStore.
        /// @param maxEvent The maximum number of events to allow.
        void SharedDataStore::SetMaxEvent(uint16_t maxEvent){
            maxEvent_ = maxEvent;
            logger::Logger::LOG("SharedDataStore::SetMaxEvent", "Max event set to: " + std::to_string(maxEvent));
        }

        /// @brief getter method for sending max events
        /// @return maxEvent_
        uint16_t SharedDataStore::GetMaxEvent(){
            return maxEvent_;
        }

        /// @brief Sets the memory limit for the SharedDataStore.
        /// @param memoryType The type of memory to set the limit for.
        /// @param maxMemoryLimit The maximum limit for the specified memory type.
        void SharedDataStore::SetMemoryLimit(std::string memoryType, uint16_t maxMemoryLimit){
            if(std::find(supportedMemoryTypes_.begin(), supportedMemoryTypes_.end(), memoryType) != supportedMemoryTypes_.end()){
                memoryType_ = memoryType;
            }else{
                memoryType_ = "mb";
                logger::Logger::LOG("SharedDataStore::SetMemoryLimit", "Invalid memory type. Using default 'mb'.", logger::LOGLEVEL::WARNING_LEVEL);
            }
            logger::Logger::LOG("SharedDataStore::SetMemoryLimit", "Memory config:- MemoryType: "+memoryType+"; MemoryLimit: "+std::to_string(maxMemoryLimit), logger::LOGLEVEL::DEBUG_LEVEL);
            maxMemoryLimit_ = maxMemoryLimit;
        }

        /// @brief getter method for getting memorytype and memory limit
        /// @return memoryType_, maxMemoryLimit_
        std::pair<std::string, uint16_t> SharedDataStore::GetMemoryLimit(){
            return {memoryType_, maxMemoryLimit_};
        }

        /// @brief Sets the details for the UDP Receiver.
        /// @param portNumber The port number for the UDP Receiver.
        /// @param timeOut The timeout value for the UDP Receiver.
        void SharedDataStore::SetUdpReceiverDetails(uint16_t portNumber, uint16_t timeOut){
            udpPortNumber_ = portNumber;
            udptimeOut_ = timeOut;
            logger::Logger::LOG("SharedDataStore::SetUdpReceiverDetails", "UDP Receiver details set: Port - " + std::to_string(portNumber) + ", Timeout - " + std::to_string(timeOut) + "ms");
        }

        /// @brief Sets the details for the TCP Receiver.
        /// @param portNumber The port number for the TCP Receiver.
        /// @param timeOut The timeout value for the TCP Receiver.
        void SharedDataStore::SetTcpReceiverDetails(uint16_t portNumber, uint16_t timeOut){
            tcpPortNumber_ = portNumber;
            tcptimeOut_ = timeOut;
            logger::Logger::LOG("SharedDataStore::SetTcpReceiverDetails", "TCP Receiver details set: Port - " + std::to_string(portNumber) + ", Timeout - " + std::to_string(timeOut) + "ms");
        }

        /// @brief sets the details for the TCP ClientRequestService
        /// @param portNumber 
        /// @param timeOut 
        void SharedDataStore::SetTcpClientRequestServiceDetails(uint16_t portNumber, uint16_t timeOut){
            tcpClientRequestServicePortNumber_ = portNumber;
            tcpClientRequestServicetimeOut_= timeOut;
            logger::Logger::LOG("SharedDataStore::SetTcpClientRequestServiceDetails", "TCP ClientRequestService Receiver details set: Port - " + std::to_string(portNumber) + ", Timeout - " + std::to_string(timeOut) + "ms");
        }
        /// @brief getter method for getting udp port number
        /// @return udpPortNumber_ 
        uint16_t SharedDataStore::GetUdpPortNumber(){
            return udpPortNumber_;
        }

        /// @brief getter method for gettign tcp port number
        /// @return 
        uint16_t SharedDataStore::GetTcpPortNumber(){
            return tcpPortNumber_;
        }

        /// @brief getter method for getting udp timeout details
        /// @return 
        uint16_t SharedDataStore::GetUdpTimeOut(){
            return udptimeOut_;
        }

        /// @brief getter method for getting tcp timeout details
        /// @return 
        uint16_t SharedDataStore::GetTcpTimeOut(){
            return tcptimeOut_;
        }

        /// @brief getter method for getting TcpClientRequestServicePortNumber
        /// @return tcpClientRequestServicePortNumber_
        uint16_t SharedDataStore::GetTcpClientRequestServicePortNumber(){
            return tcpClientRequestServicePortNumber_;
        }

        /// @brief getter method for getting TcpClientRequestService timeout
        /// @return tcpClientRequestServicetimeOut_ 
        uint16_t SharedDataStore::GetTcpClientRequestServiceTimeOut(){
            return tcpClientRequestServicetimeOut_;
        }
    }
}
