#include "ClientRequestService/ClientRequestService.hpp"

namespace sensormoniteringhub{
    namespace clientrequestservice{
        void ClientRequestService::StartService()
        {
            uint16_t portNumber;
            uint16_t timeOut;
            auto sharedDataStoreInstance{
                std::dynamic_pointer_cast<systemcontext::SharedDataStore>(
                    systemcontext::ComponentRegistry::GetComponent("SharedDataStore")
                )                
            };
            if(sharedDataStoreInstance){
                portNumber = sharedDataStoreInstance->GetTcpClientRequestServicePortNumber();
                timeOut = sharedDataStoreInstance->GetTcpClientRequestServiceTimeOut();
            }else{
                logger::Logger::LOG("ClientRequestService::StartService","SharedDataStore instance is not available!",logger::LOGLEVEL::ERROR_LEVEL);
            }
            auto TcpReceiverInstance{
                std::dynamic_pointer_cast<networkinterfacemanager::TCPReceiver>(
                    systemcontext::ComponentRegistry::GetComponent("TCPReceiver")
                )
            };
            if(TcpReceiverInstance){
                int clientRequestServiceServerSocket{TcpReceiverInstance->CreateAndBindTcpSocket(portNumber)};
                if(clientRequestServiceServerSocket != -1){
                    TcpReceiverInstance->StartReceivingClientRequestServiceThread(clientRequestServiceServerSocket, timeOut);
                }else{
                    logger::Logger::LOG("ClientRequestService::StartService", "Socket creation and binding failed!", logger::LOGLEVEL::ERROR_LEVEL);    
                }
            }else{
                logger::Logger::LOG("ClientRequestService::StartService","TCP receiver instance is not available!",logger::LOGLEVEL::ERROR_LEVEL);
            }
        }

        void ClientRequestService::StopService()
        {
        }
        void ClientRequestService::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("ClientRequestService", std::make_shared<ClientRequestService>());
            logger::Logger::LOG("ClientRequestService::Initialize", "Initialization successful!");
        }

        void ClientRequestService::Finalize()
        {
        }

        std::string ClientRequestService::HandleRequest(std::string requestStr){
            logger::Logger::LOG("ClientRequestService::HandleRequest","Handling request from client");
            // @todo
            return "";
        }
    }
}