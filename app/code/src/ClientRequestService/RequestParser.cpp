#include "ClientRequestService/RequestParser.hpp"

namespace sensormoniteringhub{
    namespace clientrequestservice{
        void RequestParser::StartService()
        {
        }
        void RequestParser::StopService()
        {
        }
        void RequestParser::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("RequestParser", std::make_shared<RequestParser>());
            logger::Logger::LOG("RequestParser::Initialize", "Initialization successful!");
        }

        bool RequestParser::ParseRequest(std::string const& reqStr, RequestData& reqData){
            auto jsonParserInstance{
                std::dynamic_pointer_cast<jsonparser::JsonParser>(
                    systemcontext::ComponentRegistry::GetComponent("JsonParser")
                )
            };
            if(!jsonParserInstance){
                logger::Logger::LOG("RequestParser::ParseRequest", "Json Parser Instance not available", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            if(jsonParserInstance->ParseRequestFromTCPClient(reqStr, reqData)){
                logger::Logger::LOG("RequestParser::ParseRequest", "Json parsing successful", logger::LOGLEVEL::DEBUG_LEVEL);
            }else{
                logger::Logger::LOG("RequestParser::ParseRequest", "Json parsing failed for the received request from client", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            auto dataPoolInstance{
                std::dynamic_pointer_cast<datapool::DataPool>(
                    systemcontext::ComponentRegistry::GetComponent("DataPool")
                )
            };
            if(!dataPoolInstance){
                logger::Logger::LOG("RequestParser::ParseRequest", "DataPool instance not available", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
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
                    // case 3
                    if(isZoneIdPresent && isTimeStampPresent){
                        auto filteredSensorData{
                            dataPoolInstance->GetEventsBasedOnZoneAndTimeStamp(reqData, isLimitPresent)
                        };
                        std::cout<<"[DEBUG] STL size: "<<filteredSensorData.size()<<std::endl;
                        // @todo
                    }else if(isZoneIdPresent){ //case 1
                        // @todo
                    }else if(isTimeStampPresent){ //case 2
                        // @todo
                    }
                }else{
                    logger::Logger::LOG("RequestParser::ParseRequest", "Invalid Request", logger::LOGLEVEL::ERROR_LEVEL);
                }
                break;
            }
            case RequestType::GET_LATEST:
            {
                // @todo
                break;
            }
            case RequestType::GET_SENSOR_STATUS:
            {
                // @todo
                break;
            }
            case RequestType::GET_STATS:
            {
                // @todo
                break;
            }
            case RequestType::GET_ZONES:
            {
                // @todo
                break;
            }
            default:
                logger::Logger::LOG("RequestParser::ParseRequest", "UNKNOWN request type", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
                break;
            }

            return true;
        }

        /// @brief helper method for validating the parsed response
        /// @param reqData 
        /// @param isZoneIdPresent 
        /// @param isTimeStampPresent 
        /// @param isLimitPresent 
        /// @return 
        bool RequestParser::ValidateEventGetEvents(RequestData const& reqData, bool& isZoneIdPresent, bool& isTimeStampPresent, bool& isLimitPresent){
            isZoneIdPresent = (reqData.zone_id_ != "");
            isLimitPresent = (reqData.limit_ != 0U);
            isTimeStampPresent = (reqData.from_time_ != 0U && reqData.to_time_ != 0U);
            return isZoneIdPresent || isTimeStampPresent;
        }

        void RequestParser::Finalize()
        {
        }
    }
}


