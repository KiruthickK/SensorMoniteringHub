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
    }
}
