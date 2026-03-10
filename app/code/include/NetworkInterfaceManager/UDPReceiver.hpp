#ifndef UDPRECEIVER_HEADER
#define UDPRECEIVER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>
#include <TimerService/TimerService.hpp>
#include <Logger/Logger.hpp>
#include <thread>
#include <queue>
#include <atomic>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

namespace sensormoniteringhub{
    namespace networkinterfacemanager{
        class UDPReceiver : public IEvents{
            private:
            uint16_t portNumber_;
            uint16_t timeOutSeconds_;
            int sock_{-1};
            std::thread udpReceiverThread_;
            std::thread udpReceivedDataHandleToSdrThread_;
            std::queue<std::vector<uint8_t>> receivedDataBufferQueue_;
            std::atomic<bool> udpThreadStopSignal_;
            sockaddr_in activeSender_;
            bool hasActiveSender_;
            void setPortNumber(uint16_t portNumber);
            void setTimeoutSeconds(uint16_t seconds);
            bool CreateAndBindSocket();
            void UdpReceiverLoop();
            bool IsHandshake(const std::string& msg);
            void SendReply(const sockaddr_in& sender, const std::string& msg);
            bool IsSameSender(const sockaddr_in& sender);
            uint64_t lastDataReceivedTimeStamp_;

            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
        };
    }
}

#endif