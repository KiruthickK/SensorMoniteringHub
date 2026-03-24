#ifndef TIMERSERVICE_HEADER
#define TIMERSERVICE_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>
#include <chrono>
#include <cstdint>

namespace sensormoniteringhub{
    namespace timerservice{
        class TimerService : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            uint64_t GetCurrentTimestamp();
            uint64_t GetElapsedSeconds(uint64_t pastTimestamp);
        };
    }
}

#endif