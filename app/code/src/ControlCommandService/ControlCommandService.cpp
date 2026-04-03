#include <ControlCommandService/ControlCommandService.hpp>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        void ControlCommandService::Initialize(){
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("ControlCommandService", std::make_shared<ControlCommandService>());
            logger::Logger::LOG("ControlCommandService::Initialize", "Initialization successful!");
        }
        void ControlCommandService::Finalize(){

        }
        void ControlCommandService::StartService(){

        }

        void ControlCommandService::StopService(){
            
        }
        std::string ControlCommandService::HandleControlCommand(const std::string& command){
            logger::Logger::LOG("ControlCommandService::HandleControlCommand", "Command received: " + command);
            std::string responseStr{""};
            auto commandParserInstance{
                std::dynamic_pointer_cast<CommandParser>(
                    systemcontext::ComponentRegistry::GetComponent("CommandParser")
                )
            };
            if(!commandParserInstance){
                logger::Logger::LOG("ControlCommandService::HandleControlCommand", "Command Parser Instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return "-1";
            }
            auto comandResponseEncoderInstance{
                std::dynamic_pointer_cast<CommandResponseEncoder>(
                    systemcontext::ComponentRegistry::GetComponent("CommandResponseEncoder")
                )
            };
            if(!comandResponseEncoderInstance){
                logger::Logger::LOG("ControlCommandService::HandleControlCommand", "Command Response Encoder Instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return "-1";
            }
            auto eventDispatcherInstance{
                std::dynamic_pointer_cast<eventdispatcher::EventDispatcher>(
                    systemcontext::ComponentRegistry::GetComponent("EventDispatcher")
                )
            };
            if(!eventDispatcherInstance){
                return comandResponseEncoderInstance->GetFailureResponse(command, "INTERNAL ERROR");
            }
            CommandOrderInformation currentCommandOrderInformation;
            if(commandParserInstance->ParseCommand(command, currentCommandOrderInformation)){
                if(currentCommandOrderInformation.type_ == CommandType::CLEAR_EVENTS){
                    if(eventDispatcherInstance->OnClearEvents()){
                        logger::Logger::LOG("ControlCommandService::HandleControlCommand", "Events Cleared!");
                        responseStr = comandResponseEncoderInstance->GetSuccessResponse(command);
                    }else{
                        responseStr = comandResponseEncoderInstance->GetFailureResponse(command, "CLEAR FAILED");
                    }
                } else if(currentCommandOrderInformation.type_ == CommandType::SHUTDOWN_REQUEST){
                    if(eventDispatcherInstance->OnShutDownRequest()){
                        responseStr = comandResponseEncoderInstance->GetSuccessResponse(command);
                    }else{
                        responseStr = comandResponseEncoderInstance->GetFailureResponse(command, "SHUTDOWN FAILED");
                    }
                } else if(currentCommandOrderInformation.type_ == CommandType::CONFIG_CHANGE){
                    if(eventDispatcherInstance->OnConfigChange(command)){
                        logger::Logger::LOG("ControlCommandService::HandleControlCommand", "New Config File updated successfully!");
                        responseStr = comandResponseEncoderInstance->GetSuccessResponse(command);
                    }else{
                        responseStr = comandResponseEncoderInstance->GetFailureResponse(command, "UPDATE CONFIG FAILED");
                    }
                }
            }else{
                logger::Logger::LOG("ControlCommandService::HandleControlCommand", "Command Parsing failed!", logger::LOGLEVEL::ERROR_LEVEL);
            }
            return responseStr;
        }
    }
}