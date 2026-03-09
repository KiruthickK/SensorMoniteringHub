#include "ClientRequestService/NotificationSender.hpp"

namespace sensormoniteringhub{
    namespace clientrequestservice{
        void NotificationSender::StartService()
        {
        }

        void NotificationSender::StopService()
        {
        }
        void NotificationSender::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("NotificationSender", std::make_shared<NotificationSender>());
            logger::Logger::LOG("NotificationSender::Initialize", "Initialization successful!");
        }

        void NotificationSender::Finalize()
        {
        }
    }
}