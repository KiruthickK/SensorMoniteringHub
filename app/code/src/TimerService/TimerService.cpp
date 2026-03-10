#include "TimerService/TimerService.hpp"

namespace sensormoniteringhub{
    namespace timerservice{
        void TimerService::StartService()
        {
        }

        void TimerService::StopService()
        {
        }

        void TimerService::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("TimerService", std::make_shared<TimerService>());
            logger::Logger::LOG("TimerService::Initialize", "Initialization successful!");
        }

        void TimerService::Finalize()
        {
        }

        /// @brief returns the current timestamp in uint64
        /// @return current timestamp
        uint64_t TimerService::GetCurrentTimestamp()
        {
            auto now = std::chrono::steady_clock::now().time_since_epoch();
            return std::chrono::duration_cast<std::chrono::seconds>(now).count();
        }

        /// @brief calculates the time difference between now time and the past time which is passed as parameter
        /// @param pastTimestamp 
        /// @return time diff in seconds
        uint64_t TimerService::GetElapsedSeconds(uint64_t pastTimestamp)
        {
            uint64_t now = GetCurrentTimestamp();
            return now - pastTimestamp;
        }
    }
}
