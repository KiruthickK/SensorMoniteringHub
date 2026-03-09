#ifndef SENSOR_DATA_RECEIVER_HEADER
#define SENSOR_DATA_RECEIVER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>

namespace sensormoniteringhub{
    namespace sensordatareceiver{
        class SensorDataReceiver : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
        };
    }
}

#endif