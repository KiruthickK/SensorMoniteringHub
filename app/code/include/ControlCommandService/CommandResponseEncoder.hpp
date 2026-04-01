#ifndef COMMAND_RESPONSE_ENCODER
#define COMMAND_RESPONSE_ENCODER

#include <Events/IEvents.hpp>
#include <Logger/Logger.hpp>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        class CommandResponseEncoder: public IEvents {
            public:
            static void Initialize();
            static void Finalize();
            virtual void StartService();
            virtual void StopService();
        };
    }
}

#endif