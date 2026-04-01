#include <ControlCommandService/CommandParser.hpp>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        void CommandParser::Initialize(){
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("CommandParser", std::make_shared<CommandParser>());
            logger::Logger::LOG("CommandParser::Initialize", "Initialization successful!");
        }
        void CommandParser::Finalize(){

        }
        void CommandParser::StartService(){

        }

        void CommandParser::StopService(){
            
        }
    }
}