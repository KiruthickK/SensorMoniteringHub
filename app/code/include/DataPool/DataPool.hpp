#ifndef DATAPOOL_HEADER
#define DATAPOOL_HEADER

#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>
#include <SensorDataReceiver/SensorData.hpp>

namespace sensormoniteringhub{
    namespace datapool{
        class DataPool : public IEvents{
            private:
            sensordatareceiver::SensorData lastReceivedSensorData_;
            std::vector<sensordatareceiver::SensorData> receivedSensorDataContainer_;
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            void WriteDataFromUDPSensorsToDataPool(sensordatareceiver::SensorData sensorData);
        };
    }
}

#endif