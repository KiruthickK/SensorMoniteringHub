#ifndef REQUEST_PARSER_HEADER
#define REQUEST_PARSER_HEADER
#include <Events/IEvents.hpp>
#include <ClientRequestService/RequestData.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>
#include <DataPool/DataPool.hpp>
#include <SensorDataReceiver/SensorData.hpp>
#include <JsonParser/JsonParser.hpp>

namespace sensormoniteringhub{
    namespace clientrequestservice{
        class RequestParser : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            bool ParseRequest(std::string const& reqStr, RequestData& reqData, std::vector<sensordatareceiver::SensorData> &responseDataContainer);
            bool ValidateEventGetEvents(RequestData const& reqData, bool& isZoneIdPresent, bool& isTimeStampPresent, bool& isLimitPresent);
        };
    }
}

#endif