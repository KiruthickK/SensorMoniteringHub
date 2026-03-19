#ifndef TCPRECEIVER_HEADER
#define TCPRECEIVER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <ClientRequestService/ClientRequestService.hpp>
#include <Logger/Logger.hpp>
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
            std::atomic<bool> tcpClientRequestServiceReceiverStopSignal_;
            int tcpClientReceiverServiceSocket_;
            void TcpReceiverLoopForClientRequestService(int const clientRequestServiceSock, uint16_t const timeOut);
            void HandleCurrentClient(int clientSocket);
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