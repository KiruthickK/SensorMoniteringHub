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
            auto commandParserInstance{
                std::dynamic_pointer_cast<CommandParser>(
                    systemcontext::ComponentRegistry::GetComponent("CommandParser")
                )
            };
            if(!commandParserInstance){
                logger::Logger::LOG("ControlCommandService::HandleControlCommand", "Command Parser Instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
            }
            CommandOrderInformation currentCommandOrderInformation;
            if(commandParserInstance->ParseCommand(command, currentCommandOrderInformation)){
                /**
                 * @todo next steps
                 */
            }else{
                logger::Logger::LOG("ControlCommandService::HandleControlCommand", "Command Parsing failed!", logger::LOGLEVEL::ERROR_LEVEL);
            }
            return command;
        }
    }
}