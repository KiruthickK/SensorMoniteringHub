#include "DataPool/DataPool.hpp"

namespace sensormoniteringhub{
    namespace datapool{
        DataPool::DataPool() : sensorDataSize(sizeof(sensordatareceiver::SensorData)) {}
        void DataPool::StartService()
        {
            auto sharedDataStoreInstance{
                std::dynamic_pointer_cast<systemcontext::SharedDataStore>(
                    systemcontext::ComponentRegistry::GetComponent("SharedDataStore")
                )
            };
            if(sharedDataStoreInstance){
                auto pair{sharedDataStoreInstance->GetMemoryLimit()};
                maxEvents_ = sharedDataStoreInstance->GetMaxEvent();
                maxMemoryLimit_ = pair.second;
                memoryType_ = pair.first;
                /**
                 * If the memory type is mb, then method to convert into bytes from mb will be mapped with
                 * calculateSize function pointer, else kb method will be mapped
                 * We are setting the max memory in terms of bytes, so it will be easy for updating every time
                 */
                std::function<size_t(uint16_t)> calculateSize = (!memoryType_.compare("mb") 
                                                                    ? std::function<size_t(uint16_t)>(MemoryConfig::FromMB) 
                                                                    : std::function<size_t(uint16_t)>(MemoryConfig::FromKB)
                                                                );
                logger::Logger::LOG("DataPool::StartService", "Memory config ["+memoryType_+":"+std::to_string(maxMemoryLimit_)+"]", logger::LOGLEVEL::DEBUG_LEVEL);
                logger::Logger::LOG("DataPool::StartService", "sensor data size in bytes ["+std::to_string(sensorDataSize)+"]", logger::LOGLEVEL::DEBUG_LEVEL);
                maxMemoryUsageBytes_.maxBytes = calculateSize(maxMemoryLimit_ * sensorDataSize);
                logger::Logger::LOG("DataPool::StartService", "Calculated Max memory bytes ["+std::to_string(maxMemoryUsageBytes_.maxBytes)+"]", logger::LOGLEVEL::DEBUG_LEVEL);
            }else{
                logger::Logger::LOG("DataPool::StartService", "Shared Data store instance not available", logger::LOGLEVEL::ERROR_LEVEL);
            }
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
            if(CheckSpaceAndEventStorageAvailability()){
                receivedSensorDataContainer_.push_back(sensorData);
                uniqueZoneIds_.insert(sensorData.zoneId_);
                currentMemoryUsageBytes_ += sensorDataSize;
                currEventCount_++;
                logger::Logger::LOG("DataPool::WriteDataFromUDPSensorsToDataPool", "Sensor data write successful", logger::LOGLEVEL::DEBUG_LEVEL);
            } else {
                logger::Logger::LOG("DataPool::WriteDataFromUDPSensorsToDataPool", ((currEventCount_ >= maxEvents_) ? "Maximum sensor data received" : "Maximum Memory limit reached for storing sensor data"
                                    ". Ignoring the Received sensor data"), logger::LOGLEVEL::ERROR_LEVEL);
                logger::Logger::LOG("DataPool::WriteDataFromUDPSensorsToDataPool", "Used memory: "+std::to_string(currentMemoryUsageBytes_)+"; Max memory: "+std::to_string(maxMemoryUsageBytes_.maxBytes)
                                    , logger::LOGLEVEL::ERROR_LEVEL);
            }
        }
        
        /// @brief pair<cur events count, current bytes usage>
        /// @return currentEvent count and Current Memory usage in bytes
        std::pair<uint16_t, size_t> DataPool::GetStats(){
            std::pair<uint16_t, size_t> result;
            {
                std::lock_guard<std::mutex> lock(receivedSensorDataContainerMutex_);
                result = {currEventCount_, currentMemoryUsageBytes_};
            }
            return result;
        }

        /// @brief getter method for getting unique zone ids
        /// @return uniqueZoneIds_
        std::set<std::string> DataPool::getUniqueZones(){
            return uniqueZoneIds_;
        }

        /// @brief method to calculate the space availablity
        /// @return true if the new sensor data can be stored
        bool DataPool::CheckSpaceAndEventStorageAvailability(){
            bool result{false};
            /**
             * No need to acquire lock again. In WriteDataFromUDPSensorsToDataPool method, lock is 
             * already aquired before calling this helper method.
             */
            if(((currEventCount_ + 1) < maxEvents_) && ((currentMemoryUsageBytes_ + sensorDataSize) < maxMemoryUsageBytes_.maxBytes)){
                result = true;
            }
            return result;
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

