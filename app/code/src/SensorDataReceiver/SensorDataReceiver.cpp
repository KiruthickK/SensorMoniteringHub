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

        void SensorDataReceiver::ProcessReceivedUdpData(std::vector<uint8_t> rawData){
            if(rawData.empty()){
                // log error
                logger::Logger::LOG("SensorDataReceiver::ProcessReceivedUdpData", "received empty raw data", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            std::string jsonString(rawData.begin(), rawData.end());
            nlohmann::json parsedJson_;
            try
            {
                parsedJson_ = nlohmann::json::parse(jsonString);
            }
            catch(const std::exception& e)
            {
                logger::Logger::LOG("SensorDataReceiver::ProcessReceivedUdpData",  e.what(), logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            logger::Logger::LOG("JSON[TEST]",parsedJson_.dump() );
            
        }

        void SensorDataReceiver::Finalize()
        {
        }
    }
}
