#ifndef COMMAND_PARSER_HEADER
#define COMMAND_PARSER_HEADER

#include <Events/IEvents.hpp>
#include <Logger/Logger.hpp>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        class CommandParser: public IEvents {
            public:
            static void Initialize();
            static void Finalize();
            virtual void StartService();
            virtual void StopService();
        };
    }
}
#endif