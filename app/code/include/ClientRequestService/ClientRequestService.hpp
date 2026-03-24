#ifndef CLIENTREQUESTSERVICE_HEADER
#define CLIENTREQUESTSERVICE_HEADER
#include <Events/IEvents.hpp>
#include <ClientRequestService/IClientRequestHandler.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>
#include <ClientRequestService/RequestData.hpp>
#include <ClientRequestService/RequestParser.hpp>
#include <ClientRequestService/ResponseEncoder.hpp>
#include <NetworkInterfaceManager/TCPReceiver.hpp>
#include <NetworkInterfaceManager/UDPReceiver.hpp>
#include <DataPool/DataPool.hpp>
#include <Logger/Logger.hpp>

namespace sensormoniteringhub{
    namespace clientrequestservice{
        class ClientRequestService : public IEvents, public IClientRequestHandler{
            private:
            bool ValidateEventGetEvents(RequestData const &reqData, bool &isZoneIdPresent, bool &isTimeStampPresent, bool &isLimitPresent);
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            virtual std::string HandleRequest(const std::string& request);
        };
    }
}

#endif