#ifndef NOTIFICATION_SENDER_HEADER
#define NOTIFICATION_SENDER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>
#include <NetworkInterfaceManager/TCPReceiver.hpp>
#include <Logger/Logger.hpp>

namespace sensormoniteringhub{
    namespace clientrequestservice{
        class ClientRequestService : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
        };
    }
}

#endif