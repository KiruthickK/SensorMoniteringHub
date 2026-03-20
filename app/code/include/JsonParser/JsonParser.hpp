#ifndef JSONPARSER_HEADER
#define JSONPARSER_HEADER

#include <Events/IEvents.hpp>
#include <nlohmann/json.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>
#include <SensorDataReceiver/SensorData.hpp>
#include <ClientRequestService/RequestData.hpp>


namespace sensormoniteringhub{
    namespace jsonparser{
        class JsonParser : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            nlohmann::json ParseJsonFromString(std::string const& jsonString);
            bool ParseAndValidateUDPSensorData(std::string const& jsonString, sensordatareceiver::SensorData& data);
            bool ParseRequestFromTCPClient(std::string const& jsonString, clientrequestservice::RequestData& data);
        };
    }
}

#endif