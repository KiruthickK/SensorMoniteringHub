#include "SensorDataReceiver/SensorDataReceiver.hpp"

namespace sensormoniteringhub{
    namespace sensordatareceiver{
        void SensorDataReceiver::StartService()
        {
        }

        void SensorDataReceiver::StopService()
        {
        }

        void SensorDataReceiver::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("SensorDataReceiver", std::make_shared<SensorDataReceiver>());
            logger::Logger::LOG("SensorDataReceiver::Initialize", "Initialization successful!");
        }

        void SensorDataReceiver::Finalize()
        {
        }
    }
}
