#include <ControlCommandService/CommandParser.hpp>

namespace sensormoniteringhub{
    namespace controlcommandservice{

        /// @brief method for initializing the component
        void CommandParser::Initialize(){
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("CommandParser", std::make_shared<CommandParser>());
            logger::Logger::LOG("CommandParser::Initialize", "Initialization successful!");
        }

        /// @brief method for parsing the command with the help of json parser module
        /// @param command 
        /// @param commandType 
        /// @return true if valid command
        bool CommandParser::ParseCommand(std::string const& command, CommandOrderInformation& commandInfo){
            auto jsonParserInstance{
                std::dynamic_pointer_cast<jsonparser::JsonParser>(
                    systemcontext::ComponentRegistry::GetComponent("JsonParser")
                )
            };
            if(!jsonParserInstance){
                logger::Logger::LOG("CommandParser::ParseCommand", "JSON Parser instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
            }
            if(jsonParserInstance->ParseCommandFromTCPClient(command, commandInfo)){
                if(commandInfo.orderId_.empty()){
                    logger::Logger::LOG("CommandParser::ParseCommand", "Invalid Order ID", logger::LOGLEVEL::WARNING_LEVEL);
                    return false;
                }
                if(commandInfo.type_ == CommandType::UNKNOWN){
                    logger::Logger::LOG("CommandParser::ParseCommand", "Invalid Order Type", logger::LOGLEVEL::WARNING_LEVEL);
                    return false;
                }
                logger::Logger::LOG("CommandParser::ParseCommand", "Command Parsed successfully. Received Command Order ID: "+commandInfo.orderId_+" Received Command:"
                    " " + ((commandInfo.type_ == CommandType::CLEAR_EVENTS) ? "CLEAR_EVENTS" : "TODO")); /** @todo After adding more commands, have to edit this log  */
                return true;
            }else{
                logger::Logger::LOG("CommandParser::ParseCommand", "Command parsing failed!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
        }

        void CommandParser::Finalize(){

        }
        void CommandParser::StartService(){

        }

        void CommandParser::StopService(){
            
        }
    }
}