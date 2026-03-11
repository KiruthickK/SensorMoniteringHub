#ifndef SENSOR_DATA_RECEIVER_HEADER
#define SENSOR_DATA_RECEIVER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <nlohmann/json.hpp>
#include <Logger/Logger.hpp>
#include <vector>
#include <stdint.h>

namespace sensormoniteringhub{
    namespace sensordatareceiver{
        class SensorDataReceiver : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            void ProcessReceivedUdpData(std::vector<uint8_t> rawData);
        };
    }
}

#endif