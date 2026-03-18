#ifndef SHARED_DATA_STORE_HEADER
#define SHARED_DATA_STORE_HEADER

#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>
#include <vector>
#include <algorithm>

namespace sensormoniteringhub{
    namespace systemcontext{
        /// @brief SharedDataStore class responsible for storing and managing shared data across different components of the Sensor Monitoring Hub Manager, including configuration parameters and other relevant information.
        class SharedDataStore : public IEvents{
            private:
            uint16_t maxEvent_;
            uint16_t currEventCount_;
            uint16_t maxMemoryLimit_;
            uint16_t currMemoryUsage_;
            std::string memoryType_;
            std::vector<std::string> supportedMemoryTypes_{"mb", "kb"};
            uint16_t udpPortNumber_;
            uint16_t udptimeOut_;
            uint16_t tcpPortNumber_;
            uint16_t tcptimeOut_;
            uint16_t tcpClientRequestServicePortNumber_;
            uint16_t tcpClientRequestServicetimeOut_;
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            void SetMaxEvent(uint16_t maxEvent);
            void SetMemoryLimit(std::string memoryType, uint16_t maxMemoryLimit);
            void SetUdpReceiverDetails(uint16_t portNumber, uint16_t timeOut);
            void SetTcpReceiverDetails(uint16_t portNumber, uint16_t timeOut);
            void SetTcpClientRequestServiceDetails(uint16_t portNumber, uint16_t timeOut);
            uint16_t GetUdpPortNumber();
            uint16_t GetTcpPortNumber();
            uint16_t GetTcpClientRequestServicePortNumber();
            uint16_t GetUdpTimeOut();
            uint16_t GetTcpTimeOut();
            uint16_t GetTcpClientRequestServiceTimeOut();
        };
    }
}

#endif