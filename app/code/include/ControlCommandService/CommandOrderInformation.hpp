#ifndef COMMAND_ORDER_INFORMATION_HEADER
#define COMMAND_ORDER_INFORMATION_HEADER

#include <iostream>
#include <stdint.h>

namespace sensormoniteringhub{
    namespace controlcommandservice{

        /// @brief enumeration class to represent the commadn type
        enum class CommandType : uint8_t{
            CLEAR_EVENTS = 1,
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
            }
            return CommandType::UNKNOWN;
        }
    }
}
#endif