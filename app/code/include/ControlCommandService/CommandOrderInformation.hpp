#ifndef COMMAND_ORDER_INFORMATION_HEADER
#define COMMAND_ORDER_INFORMATION_HEADER

#include <iostream>
#include <stdint.h>

namespace sensormoniteringhub{
    namespace controlcommandservice{

        /// @brief enumeration class to represent the commadn type
        enum class CommandType : uint8_t{
            CLEAR_EVENTS = 1,
            CONFIG_CHANGE = 2,
            SHUTDOWN_REQUEST = 3,
            UNKNOWN = 100
        };

        /// @brief struct to store Command Order information
        struct CommandOrderInformation{
            CommandType type_;
            std::string orderId_;
        };

        /// @brief method for converting string command into enum class type of command
        /// @param commandStr 
        /// @return 
        inline CommandType GetCommandType(std::string const& commandStr){
            if(commandStr == std::string_view{"CLEAR_EVENTS"}){
                return CommandType::CLEAR_EVENTS;
            }else if(commandStr == std::string_view{"CONFIG_CHANGE"}){
                return CommandType::CONFIG_CHANGE;
            }else if(commandStr == std::string_view{"SHUTDOWN_REQUEST"}){
                return CommandType::SHUTDOWN_REQUEST;
            }
            return CommandType::UNKNOWN;
        }
    }
}
#endif