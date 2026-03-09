#ifndef TCPRECEIVER_HEADER
#define TCPRECEIVER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>

namespace sensormoniteringhub{
    namespace networkinterfacemanager{
        class TCPReceiver : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
        };
    }
}

#endif