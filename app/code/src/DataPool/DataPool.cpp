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
        /// @param filteredSensorData container to get filled with data if rpesent
        /// @param isZoneIdPresent flag to notify whether to filter based on zone id or not
        /// @param isTimeStampPresent flag to notify whether to filter based on timestamp or not
        /// @param isLimitedResults flag to notify whether the results are limmited or not
        /// @return true if data is present, else false 
        bool DataPool::GetEventsBasedOnZoneAndTimeStamp(clientrequestservice::RequestData const& reqData, std::vector<sensordatareceiver::SensorData> &filteredSensorData, bool const isZoneIdPresent, bool const isTimeStampPresent, bool const isLimitedResults){
            filteredSensorData.clear();
            std::vector<sensordatareceiver::SensorData> localCopyOfreceivedSensorDataContainer;
            {
                std::lock_guard<std::mutex> lock(receivedSensorDataContainerMutex_);
                localCopyOfreceivedSensorDataContainer = receivedSensorDataContainer_;
            }
            uint64_t currentLimit{0U};
            for(auto const& sd: localCopyOfreceivedSensorDataContainer){
                /**
                 * If timestamp alone is present, then the data should filter based only on timestamp alone,
                 * If Zone id alone is present, then the data should filter based only on zone id,
                 * If both are present, then based on both the data should be filtered,
                 * Limit is optional, if present, we have to limit the data, else let the complete results
                 * to shared in the response
                 */
                bool filterResult{
                    ((isTimeStampPresent && isZoneIdPresent) && 
                        (
                            ((sd.timeStamp_ >= reqData.from_time_) && (sd.timeStamp_ <= reqData.to_time_)) && 
                            (sd.zoneId_ == reqData.zone_id_)
                        )
                    ) ||
                    (
                        isTimeStampPresent && 
                        (
                            (sd.timeStamp_ >= reqData.from_time_) && (sd.timeStamp_ <= reqData.to_time_)
                        )
                    ) || 
                    (isZoneIdPresent && (sd.zoneId_ == reqData.zone_id_))
                };
                if(filterResult){
                    filteredSensorData.push_back(sd);
                    currentLimit++;
                    if(isLimitedResults && currentLimit >= reqData.limit_){
                        break;
                    }
                }
            }
            return !filteredSensorData.empty();
        }

        /// @brief returns the latest sensor data received by the application
        /// @param sensorData 
        /// @return true if data is available
        bool DataPool::GetLastReceivedData(sensordatareceiver::SensorData& sensorData){
            {
                std::lock_guard<std::mutex> lock(receivedSensorDataContainerMutex_);
                if(lastReceivedSensorData_.sensorId_.empty()){
                    logger::Logger::LOG("DataPool::GetLastReceivedData", "No data available", logger::LOGLEVEL::WARNING_LEVEL);
                    return false;
                }
            sensorData = lastReceivedSensorData_;
            }
            return true;
        }

        void DataPool::Finalize()
        {
        }
    }
}

