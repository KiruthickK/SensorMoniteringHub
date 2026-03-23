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

        /// @brief Method for parsing the request with help of jsonparser module
        /// @param reqStr 
        /// @param reqData 
        /// @return true if parsing success 
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
            return true;
        }


        void RequestParser::Finalize()
        {
        }
    }
}


