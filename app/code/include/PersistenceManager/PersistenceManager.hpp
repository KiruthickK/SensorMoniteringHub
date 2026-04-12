#ifndef PERSISTENCEMANAGER_HEADER
#define PERSISTENCEMANAGER_HEADER


#include <sqlite3.h>
#include <iostream>
#include <filesystem>
#include <SensorDataReceiver/SensorData.hpp>
#include <vector>

namespace sensormoniteringhub{
    namespace persistencemanager{
        /**
         * PersistenceManager has exactly one consumer — DataPool
         * So here we gonna follow composition.
         * PersistenceManager will be handled by DataPool
         */
        class PersistenceManager {
        private:
            std::string const dbPath_{"../data/"};
            std::string const dbName_{"smh.db"};
            uint64_t maxEvents_;
            uint64_t curEvents_;
            std::string const dbTableName_{"SENSOR_DATA_TABLE"};
            sqlite3* dbHandler_{nullptr};
            bool OpenDataBase();
            bool CheckTableExistsInDb();
            bool CreateTableInDb();
            bool CleanupData();
        public:
            void StartService();
            void StopService();
            void SetMaxEvents(uint64_t maxEvents);
            bool GetRecoveredSensorData(std::vector<sensordatareceiver::SensorData> &sensorDataContainer);
            void PersistSensorData(std::vector<sensordatareceiver::SensorData> const &sensorDataContainer);
        };
    }
}

#endif