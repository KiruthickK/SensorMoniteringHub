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

        std::string ClientRequestService::HandleRequest(const std::string& request) {
            logger::Logger::LOG("ClientRequestService::HandleRequest","Handling request from client");
            RequestData reqData;
            auto reqParserInstance{
                std::dynamic_pointer_cast<RequestParser>(systemcontext::ComponentRegistry::GetComponent("RequestParser"))
            };
            if(!reqParserInstance){
                logger::Logger::LOG("ClientRequestService::HandleRequest","Request parser instance not available",logger::LOGLEVEL::ERROR_LEVEL);
                return "-1"; // get internal error response from response encoder and send the response @todo
            }
            bool reqParsedResult{
                reqParserInstance->ParseRequest(request, reqData)
            };
            if(!reqParsedResult){
                logger::Logger::LOG("ClientRequestService::HandleRequest","Request parser instance not available",logger::LOGLEVEL::ERROR_LEVEL);
                return "-1"; // get invalid request from responseEncoder and send the response @todo
            }
            // @todo
            return "";
        }
    }
}