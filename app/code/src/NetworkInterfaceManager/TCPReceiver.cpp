#include <NetworkInterfaceManager/TCPReceiver.hpp>

namespace sensormoniteringhub{
    namespace networkinterfacemanager{
        void TCPReceiver::StartService()
        {
        }

        void TCPReceiver::StopService()
        {
        }

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

        int TCPReceiver::CreateAndBindTcpSocket(uint16_t const portNumber){
            int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //clientRequestServiceSock_
            if (serverSocket < 0)
            {
                std::cerr << "Socket creation failed\n";
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
                std::cerr << "Bind failed\n";
                close(serverSocket);
                return -1;
            }
            logger::Logger::LOG("TCPReceiver::CreateAndBindTcpSocket", "Socket creation and binding successful!");
            return serverSocket;
        }
        void TCPReceiver::TcpReceiverLoopForClientRequestService(int const clientRequestServiceSock, uint16_t const timeOut){
            logger::Logger::LOG("TCPReceiver::TcpReceiverLoopForClientRequestService","Creating thread for receiving requests from TCP client");
            tcpClientRequestServiceReceiverThread_ = std::thread();
        }
    }
}

