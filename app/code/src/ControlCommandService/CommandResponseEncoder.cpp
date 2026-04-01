#include <ControlCommandService/CommandResponseEncoder.hpp>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        void CommandResponseEncoder::Initialize(){
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("CommandResponseEncoder", std::make_shared<CommandResponseEncoder>());
            logger::Logger::LOG("CommandResponseEncoder::Initialize", "Initialization successful!");
        }
        void CommandResponseEncoder::Finalize(){

        }
        void CommandResponseEncoder::StartService(){

        }

        void CommandResponseEncoder::StopService(){
            
        }
    }
}