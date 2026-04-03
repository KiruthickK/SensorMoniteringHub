#ifndef COMMAND_PARSER_HEADER
#define COMMAND_PARSER_HEADER

#include <Events/IEvents.hpp>
#include <Logger/Logger.hpp>
#include <JsonParser/JsonParser.hpp>
#include <ControlCommandService/CommandOrderInformation.hpp>
#include <stdint.h>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        class CommandParser: public IEvents {
            public:
            static void Initialize();
            static void Finalize();
            virtual void StartService();
            virtual void StopService();
            bool ParseCommand(std::string const& command, CommandOrderInformation &commandInfo);
        };
    }
}
#endif