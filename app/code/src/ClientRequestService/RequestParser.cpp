#include "ClientRequestService/RequestParser.hpp"

namespace sensormoniteringhub{
    namespace clientrequestservice{
        void RequestParser::StartService()
        {
        }
        void RequestParser::StopService()
        {
        }
        void RequestParser::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("RequestParser", std::make_shared<RequestParser>());
            logger::Logger::LOG("RequestParser::Initialize", "Initialization successful!");
        }

        void RequestParser::Finalize()
        {
        }
    }
}


