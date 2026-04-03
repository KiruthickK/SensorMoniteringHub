#include <ControlCommandService/CommandResponseEncoder.hpp>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        void CommandResponseEncoder::Initialize(){
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("CommandResponseEncoder", std::make_shared<CommandResponseEncoder>());
            logger::Logger::LOG("CommandResponseEncoder::Initialize", "Initialization successful!");
        }
        /// @brief method for generating failure response for tcp control client
        /// @param commandRequestStr 
        /// @param failureReasonStr 
        /// @return 
        std::string CommandResponseEncoder::GetFailureResponse(std::string const& commandRequestStr, std::string const& failureReasonStr){
            auto jsonParserInstance{
                std::dynamic_pointer_cast<jsonparser::JsonParser>(
                    systemcontext::ComponentRegistry::GetComponent("JsonParser")
                )
            };
            if(!jsonParserInstance){
                logger::Logger::LOG("ResponseEncoder::EncodeResponseToString", "JsonParser object is not available", logger::LOGLEVEL::ERROR_LEVEL);
                return "INTERNAL ERROR";
            }
            return jsonParserInstance->SerializeFailureResponseToTcpClient(commandRequestStr, failureReasonStr);
        }
        /// @brief method for generating success response for tcp control client
        /// @param commandRequestStr 
        /// @return 
        std::string CommandResponseEncoder::GetSuccessResponse(std::string const& commandRequestStr){
            auto jsonParserInstance{
                std::dynamic_pointer_cast<jsonparser::JsonParser>(
                    systemcontext::ComponentRegistry::GetComponent("JsonParser")
                )
            };
            if(!jsonParserInstance){
                logger::Logger::LOG("ResponseEncoder::EncodeResponseToString", "JsonParser object is not available", logger::LOGLEVEL::ERROR_LEVEL);
                return "INTERNAL ERROR";
            }
            return jsonParserInstance->SerializeSuccessResponseToTcpControlClient(commandRequestStr);
        }
        void CommandResponseEncoder::Finalize(){

        }
        void CommandResponseEncoder::StartService(){

        }

        void CommandResponseEncoder::StopService(){
            
        }
    }
}