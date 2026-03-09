#include "JsonParser/JsonParser.hpp"

namespace sensormoniteringhub{
    namespace jsonparser{
        void JsonParser::StartService()
        {
        }

        void JsonParser::StopService()
        {
        }

        void JsonParser::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("JsonParser", std::make_shared<JsonParser>());
            logger::Logger::LOG("JsonParser::Initialize", "Initialization successful!");
        }

        void JsonParser::Finalize()
        {
        }
    }
}
