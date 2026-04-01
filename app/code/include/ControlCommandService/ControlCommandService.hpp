#ifndef CONTROL_COMMAND_SERVICE
#define CONTROL_COMMAND_SERVICE

#include <Events/IEvents.hpp>
#include <Logger/Logger.hpp>
#include <iostream>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        class ControlCommandService: public IEvents {
            public:
            static void Initialize();
            static void Finalize();
            virtual void StartService();
            virtual void StopService();
            std::string HandleControlCommand(const std::string& command);
        };
    }
}

#endif