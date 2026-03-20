#ifndef CLIENTREQUESTSERVICE_HEADER
#define CLIENTREQUESTSERVICE_HEADER
#include <Events/IEvents.hpp>
#include <ClientRequestService/IClientRequestHandler.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>
#include <ClientRequestService/RequestData.hpp>
#include <ClientRequestService/RequestParser.hpp>
#include <NetworkInterfaceManager/TCPReceiver.hpp>
#include <Logger/Logger.hpp>

namespace sensormoniteringhub{
    namespace clientrequestservice{
        class ClientRequestService : public IEvents, public IClientRequestHandler{
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