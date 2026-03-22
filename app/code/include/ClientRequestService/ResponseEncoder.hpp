#ifndef RESPONSE_ENCODER_HEADER
#define RESPONSE_ENCODER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>
#include <SensorDataReceiver/SensorData.hpp>
#include <ClientRequestService/RequestData.hpp>
#include <JsonParser/JsonParser.hpp>

namespace sensormoniteringhub{
    namespace clientrequestservice{
        class ResponseEncoder : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            std::string EncodeResponseToString(std::vector<sensordatareceiver::SensorData> const& responseData, RequestData const& reqData);
        };
    }
}

#endif