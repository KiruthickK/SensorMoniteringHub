#ifndef DATAPOOL_HEADER
#define DATAPOOL_HEADER

#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>
#include <SensorDataReceiver/SensorData.hpp>
#include <ClientRequestService/RequestData.hpp>
#include <thread>
#include <algorithm>
#include <iterator>

namespace sensormoniteringhub{
    namespace datapool{
        class DataPool : public IEvents{
            private:
            sensordatareceiver::SensorData lastReceivedSensorData_;
            std::vector<sensordatareceiver::SensorData> receivedSensorDataContainer_;
            std::mutex receivedSensorDataContainerMutex_;
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            void WriteDataFromUDPSensorsToDataPool(sensordatareceiver::SensorData sensorData);
            std::vector<sensordatareceiver::SensorData> GetEventsBasedOnZoneAndTimeStamp(clientrequestservice::RequestData const& reqData, bool const isLimitedResults = false);
        };
    }
}

#endif