#ifndef TCPRECEIVER_HEADER
#define TCPRECEIVER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>
#include <ClientRequestService/ClientRequestService.hpp>
#include <Logger/Logger.hpp>
#include <ControlCommandService/ControlCommandService.hpp>
#include <EventDispatcher/EventDispatcher.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

namespace sensormoniteringhub{
    namespace networkinterfacemanager{
        class TCPReceiver : public IEvents{
            private:
            std::thread tcpClientRequestServiceReceiverThread_;
            std::thread tcpControlClientReceiverThread_;
            std::atomic<bool> tcpClientRequestServiceReceiverStopSignal_;
            std::atomic<bool> tcpControlClientReceiverStopSignal_;
            int tcpClientReceiverServiceSocket_{-1};
            int tcpControlClientSocket_{-1};
            uint16_t controlClientPortNumber_;
            uint16_t controlClientTimeOutSeconds_;
            void TcpReceiverLoopForClientRequestService(int const clientRequestServiceSock, uint16_t const timeOut);
            void TcpReceiverLoopForControlClient();
            void HandleCurrentRequestClient(int clientSocket);
            void HandleCurrentControlClient(int clientSocket);
            void ClearData();
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            int CreateAndBindTcpSocket(uint16_t const portNumber);
            void StartReceivingClientRequestServiceThread(int const clientRequestServiceSock, uint16_t const timeOut);
        };
    }
}

#endif