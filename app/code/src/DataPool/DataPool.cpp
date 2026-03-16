#include "DataPool/DataPool.hpp"

namespace sensormoniteringhub{
    namespace datapool{
        void DataPool::StartService()
        {
        }

        void DataPool::StopService()
        {
        }

        void DataPool::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("DataPool", std::make_shared<DataPool>());
            logger::Logger::LOG("DataPool::Initialize", "Initialization successful!");
        }

        /// @brief method which writes the received data to memory
        /// @param sensorData 
        void DataPool::WriteDataFromUDPSensorsToDataPool(sensordatareceiver::SensorData sensorData){
            // updating the latest received data
            if(lastReceivedSensorData_.timeStamp_ < sensorData.timeStamp_){
                lastReceivedSensorData_ = sensorData;
            }
            receivedSensorDataContainer_.push_back(sensorData);
        }

        void DataPool::Finalize()
        {
        }
    }
}

