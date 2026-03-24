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

        /// @brief method for handling request from client
        /// @param request 
        /// @return returns the response in string
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
            // decision logic and datapool call here @todo
            auto dataPoolInstance{
                std::dynamic_pointer_cast<datapool::DataPool>(
                    systemcontext::ComponentRegistry::GetComponent("DataPool")
                )
            };
            if(!dataPoolInstance){
                logger::Logger::LOG("RequestParser::ParseRequest", "DataPool instance not available", logger::LOGLEVEL::ERROR_LEVEL);
                return "-1";
            }
            auto responseEncoder{
                std::dynamic_pointer_cast<ResponseEncoder>(
                    systemcontext::ComponentRegistry::GetComponent("ResponseEncoder")
                )
            };
            if(!responseEncoder){
                logger::Logger::LOG("ClientRequestService::HandleRequest","Response Encoder instance not available",logger::LOGLEVEL::ERROR_LEVEL);
                return "-1"; // get invalid request from responseEncoder and send the response @todo
            }
            std::string responseStr{""};
            switch (reqData.reqType_)
            {
            case RequestType::GET_EVENTS:
            {
                /**
                 * For GET_EVENTS three combinations are allowed
                 * 1. zone_id with or without limit
                 * 2. from and to time specified with or without limit
                 * 3. combination of 1. and 2.
                 */
                bool isZoneIdPresent{false};
                bool isTimeStampPresent{false};
                bool isLimitPresent{false};
                if(ValidateEventGetEvents(reqData, isZoneIdPresent, isTimeStampPresent, isLimitPresent)){
                    std::vector<sensordatareceiver::SensorData> responseDataContainer;
                    if(!dataPoolInstance->GetEventsBasedOnZoneAndTimeStamp(reqData, responseDataContainer, isZoneIdPresent, isTimeStampPresent, isLimitPresent)){
                        logger::Logger::LOG("RequestParser::ParseRequest", "No data available for the request : " + reqData.reqId_, logger::LOGLEVEL::WARNING_LEVEL);
                        return "-1";
                    }
                    // getting encoded response (struct objects(vector) -> JSON -> string)
                    responseStr = responseEncoder->EncodeResponseToString(responseDataContainer, reqData);
                    logger::Logger::LOG("RequestParser::ParseRequest", "Data available for the request : " + reqData.reqId_);
                }else{
                    logger::Logger::LOG("RequestParser::ParseRequest", "Invalid Request", logger::LOGLEVEL::ERROR_LEVEL);
                }
                break;
            }
            case RequestType::GET_LATEST:
            {
                sensordatareceiver::SensorData latestSensorData;
                if(!dataPoolInstance->GetLastReceivedData(latestSensorData)){
                    logger::Logger::LOG("RequestParser::ParseRequest", "No data available to respond", logger::LOGLEVEL::WARNING_LEVEL);
                    return "-1";
                }
                // getting encoded response (struct object -> JSON -> string)
                responseStr = responseEncoder->EncodeResponseToString(latestSensorData, reqData);
                break;
            }
            case RequestType::GET_SENSOR_STATUS:
            {
                auto udpSensorReceiverInstance{
                    std::dynamic_pointer_cast<networkinterfacemanager::UDPReceiver>(
                        systemcontext::ComponentRegistry::GetComponent("UDPReceiver")
                    )
                };
                if(udpSensorReceiverInstance){
                    auto pair{udpSensorReceiverInstance->GetSensorEcuStatus()};
                    if(pair.second.empty()){
                        logger::Logger::LOG("RequestParser::ParseRequest", "No data available to respond", logger::LOGLEVEL::WARNING_LEVEL);
                        return "-1";
                    }
                    responseStr = responseEncoder->EncodeResponseToString(pair, reqData);
                }else{
                    logger::Logger::LOG("RequestParser::ParseRequest", "UDP Receiver instance not available", logger::LOGLEVEL::ERROR_LEVEL);
                }
                break;
            }
            case RequestType::GET_STATS:
            {
                auto pair{dataPoolInstance->GetStats()};
                if(pair.first == 0U || pair.second == 0U){
                    logger::Logger::LOG("RequestParser::ParseRequest", "No data available to respond", logger::LOGLEVEL::WARNING_LEVEL);
                    return "-1";
                }
                responseStr = responseEncoder->EncodeResponseToString(pair, reqData);
                break;
            }
            case RequestType::GET_ZONES:
            {
                auto uniqueZoneIds{dataPoolInstance->getUniqueZones()};
                if(!uniqueZoneIds.empty()){
                    responseStr = responseEncoder->EncodeResponseToString(uniqueZoneIds, reqData);
                }else{
                    logger::Logger::LOG("RequestParser::ParseRequest", "No data available to respond", logger::LOGLEVEL::WARNING_LEVEL);
                    return "-1";
                }
                break;
            }
            default:
                logger::Logger::LOG("RequestParser::ParseRequest", "UNKNOWN request type", logger::LOGLEVEL::ERROR_LEVEL);
                return "-1";
                break;
            }
            return responseStr;
        }
        /// @brief helper method for validating the parsed response
        /// @param reqData 
        /// @param isZoneIdPresent 
        /// @param isTimeStampPresent 
        /// @param isLimitPresent 
        /// @return 
        bool ClientRequestService::ValidateEventGetEvents(RequestData const& reqData, bool& isZoneIdPresent, bool& isTimeStampPresent, bool& isLimitPresent){
            isZoneIdPresent = (!reqData.zone_id_.empty());
            isLimitPresent = (reqData.limit_ != 0U);
            isTimeStampPresent = (reqData.from_time_ != 0U && reqData.to_time_ != 0U && (reqData.from_time_ < reqData.to_time_));
            return isZoneIdPresent || isTimeStampPresent;
        }
    }
}