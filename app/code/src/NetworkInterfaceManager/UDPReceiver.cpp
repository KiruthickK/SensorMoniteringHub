#include "NetworkInterfaceManager/UDPReceiver.hpp"


namespace sensormoniteringhub{
    namespace networkinterfacemanager{
        void UDPReceiver::StartService()
        {
        }

        void UDPReceiver::StopService()
        {
        }

        void UDPReceiver::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("UDPReceiver", std::make_shared<UDPReceiver>());
            logger::Logger::LOG("UDPReceiver::Initialize", "Initialization successful!");
        }

        void UDPReceiver::Finalize()
        {
        }
    }
}

