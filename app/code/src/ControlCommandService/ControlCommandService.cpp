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
            return command;
        }
    }
}