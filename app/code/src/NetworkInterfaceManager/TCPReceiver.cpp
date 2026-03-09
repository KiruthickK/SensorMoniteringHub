#include "NetworkInterfaceManager/TCPReceiver.hpp"

namespace sensormoniteringhub{
    namespace networkinterfacemanager{
        void TCPReceiver::StartService()
        {
        }

        void TCPReceiver::StopService()
        {
        }

        void TCPReceiver::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("TCPReceiver", std::make_shared<TCPReceiver>());
            logger::Logger::LOG("TCPReceiver::Initialize", "Initialization successful!");
        }

        void TCPReceiver::Finalize()
        {
        }
    }
}

