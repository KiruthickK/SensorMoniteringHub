#include "ClientRequestService/ResponseEncoder.hpp"

namespace sensormoniteringhub{
    namespace clientrequestservice{
        void ResponseEncoder::StartService()
        {
        }
        void ResponseEncoder::StopService()
        {
        }
        void ResponseEncoder::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("ResponseEncoder", std::make_shared<ResponseEncoder>());
            logger::Logger::LOG("ResponseEncoder::Initialize", "Initialization successful!");
        }

        void ResponseEncoder::Finalize()
        {
        }
    }
}

