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
#include <functional>
#include <set>

namespace sensormoniteringhub{
    namespace datapool{
        /// @brief store config limit always in bytes internally and convert at the boundary (when reading config)
        struct MemoryConfig {
            size_t maxBytes;
            static size_t FromKB(uint16_t kb) { return static_cast<size_t>(kb) * 1024; }
            static size_t FromMB(uint16_t mb) { return static_cast<size_t>(mb) * 1024 * 1024; }
        };
        class DataPool : public IEvents{
            private:
            uint16_t maxEvents_;
            size_t sensorDataSize;
            uint16_t currEventCount_;
            uint16_t maxMemoryLimit_;
            MemoryConfig maxMemoryUsageBytes_;
            size_t currentMemoryUsageBytes_;
            std::string memoryType_;
            sensordatareceiver::SensorData lastReceivedSensorData_;
            std::vector<sensordatareceiver::SensorData> receivedSensorDataContainer_;
            std::mutex receivedSensorDataContainerMutex_;
            bool CheckSpaceAndEventStorageAvailability();
            public:
            DataPool();
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            void WriteDataFromUDPSensorsToDataPool(sensordatareceiver::SensorData sensorData);
            [[nodiscard]] bool GetEventsBasedOnZoneAndTimeStamp(clientrequestservice::RequestData const& reqData, std::vector<sensordatareceiver::SensorData> &filteredSensorData, bool const isZoneIdPresent, bool const isTimeStampPresent, bool const isLimitedResults = false);
            [[nodiscard]] bool GetLastReceivedData(sensordatareceiver::SensorData& sensorData);
            /// @brief pair<cur events count, current bytes usage>
            std::pair<uint16_t, size_t> GetStats();
        };
    }
}

#endif