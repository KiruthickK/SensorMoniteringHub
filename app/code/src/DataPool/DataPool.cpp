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
            std::lock_guard<std::mutex> lock(receivedSensorDataContainerMutex_);
            receivedSensorDataContainer_.push_back(sensorData);
        }

        /// @brief method for fetching results for the query "GET_EVENTS" based on zone_id and timestamp
        /// @param reqData request data struct object
        /// @param isLimitedResults flag to notify whether the results are limmited or not
        /// @return 
        std::vector<sensordatareceiver::SensorData> DataPool::GetEventsBasedOnZoneAndTimeStamp(clientrequestservice::RequestData const& reqData, bool const isLimitedResults){
            std::vector<sensordatareceiver::SensorData> filteredSensorData;
            {
                std::lock_guard<std::mutex> lock(receivedSensorDataContainerMutex_);
                uint64_t currentLimit{0U};
                std::copy_if(receivedSensorDataContainer_.begin(), receivedSensorDataContainer_.end(), std::back_inserter(filteredSensorData), [&currentLimit, &reqData, &isLimitedResults](const sensordatareceiver::SensorData& sd){
                    bool filterResult{(sd.timeStamp_ >= reqData.from_time_ && sd.timeStamp_ <= reqData.to_time_) && (sd.zoneId_ == reqData.zone_id_)};
                    if(isLimitedResults && filterResult){
                        if(currentLimit >= reqData.limit_){
                            return false;
                        }
                        currentLimit++;
                    }
                    return filterResult;
                });
            }
            return filteredSensorData;
        }

        void DataPool::Finalize()
        {
        }
    }
}

