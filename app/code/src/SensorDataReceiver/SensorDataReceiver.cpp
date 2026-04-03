#include "SensorDataReceiver/SensorDataReceiver.hpp"

namespace sensormoniteringhub{
    namespace sensordatareceiver{
        void SensorDataReceiver::StartService()
        {
        }

        void SensorDataReceiver::StopService()
        {
        }

        void SensorDataReceiver::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("SensorDataReceiver", std::make_shared<SensorDataReceiver>());
            logger::Logger::LOG("SensorDataReceiver::Initialize", "Initialization successful!");
        }

        void SensorDataReceiver::ProcessReceivedUdpData(std::vector<uint8_t> const &rawData){
            if(rawData.empty()){
                logger::Logger::LOG("SensorDataReceiver::ProcessReceivedUdpData", "received empty raw data", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            std::string jsonString(rawData.begin(), rawData.end());
            auto jsonParserInstance{
                std::dynamic_pointer_cast<jsonparser::JsonParser>(systemcontext::ComponentRegistry::GetComponent("JsonParser"))
            };
            if(jsonParserInstance){
                SensorData data;
                if(jsonParserInstance->ParseAndValidateUDPSensorData(jsonString, data)){
                    auto dataPoolInstance_{
                        std::dynamic_pointer_cast<datapool::DataPool>(systemcontext::ComponentRegistry::GetComponent("DataPool"))
                    };
                    if(dataPoolInstance_){
                        dataPoolInstance_->WriteDataFromUDPSensorsToDataPool(data);
                    }else{
                        logger::Logger::LOG("SensorDataReceiver::ProcessReceivedUdpData","DataPool instance is not available",logger::LOGLEVEL::ERROR_LEVEL);
                    }
                }else{
                    logger::Logger::LOG("SensorDataReceiver::ProcessReceivedUdpData", "Received sensor data JSON validation Failed", logger::LOGLEVEL::WARNING_LEVEL);
                }
            }else{
                logger::Logger::LOG("SensorDataReceiver::ProcessReceivedUdpData","JsonParser instance is not available",logger::LOGLEVEL::ERROR_LEVEL);
            }
        }

        void SensorDataReceiver::Finalize()
        {
        }
    }
}
