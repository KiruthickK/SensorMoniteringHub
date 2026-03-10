#include "SensorMonitoringHubManager/ConfigParser.hpp"

namespace sensormoniteringhub{
    namespace sensormonitoringhubmanager{
        bool ConfigParser::LoadConfigs(std::string const& configPath)
        {
            auto SharedDataStoreInstance{
                std::dynamic_pointer_cast<systemcontext::SharedDataStore>(
                    systemcontext::ComponentRegistry::GetComponent("SharedDataStore")
                )
            };
            if(SharedDataStoreInstance == nullptr){
                logger::Logger::LOG("ConfigParser::LoadConfigs", "Shared Data Store instance is not available", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            logger::Logger::LOG("ConfigParser::LoadConfigs", "Started loading config files");
            logger::Logger::LOG("ConfigParser::LoadConfigs", "Config File path: " + configPath);
            std::ifstream file(configPath);
            if (!file.is_open()) {
                logger::Logger::LOG("ConfigParser::LoadConfigs", "Failed to open JSON file", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            nlohmann::json config;
            file >> config;
            if(config.contains("maxEvent") && config.at("maxEvent").is_number()){
                uint16_t maxEvents{config["maxEvent"]};
                logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded maxEvents; value=["+std::to_string(maxEvents)+"]");
                SharedDataStoreInstance->SetMaxEvent(maxEvents);
            }
            if(config.contains("memoryLimit") && config.at("memoryLimit").is_object()){
                std::string memoryType{""};
                uint16_t maxMemoryLimit;
                if(config.at("memoryLimit").contains("memoryType") 
                    && config.at("memoryLimit").at("memoryType").is_string()
                ){
                    memoryType = config.at("memoryLimit").at("memoryType");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded memoryType; value=["+memoryType+"]");
                }
                if(config.at("memoryLimit").contains("maxMemoryLimit") 
                    && config.at("memoryLimit").at("maxMemoryLimit").is_number()
                ){
                    maxMemoryLimit = config.at("memoryLimit").at("maxMemoryLimit");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded maxMemoryLimit; value=["+std::to_string(maxMemoryLimit)+"]");
                }
                SharedDataStoreInstance->SetMemoryLimit(memoryType, maxMemoryLimit);
            }
            if(config.contains("udpReceiverDetails") && config.at("udpReceiverDetails").is_object()){
                uint16_t udpPortNumber;
                uint16_t udptimeOut;
                if(config.at("udpReceiverDetails").contains("portNumber")
                    && config.at("udpReceiverDetails").at("portNumber").is_number()
                ){
                    udpPortNumber = config.at("udpReceiverDetails").at("portNumber");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded udpPortNumber; value=["+std::to_string(udpPortNumber)+"]");
                }
                if(config.at("udpReceiverDetails").contains("timeOut")
                    && config.at("udpReceiverDetails").at("timeOut").is_number()
                ){
                    udptimeOut = config.at("udpReceiverDetails").at("timeOut");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded udptimeOut; value=["+std::to_string(udptimeOut)+"]");
                }
                SharedDataStoreInstance->SetUdpReceiverDetails(udpPortNumber, udptimeOut);
            }
            if(config.contains("tcpReceiverDetails") && config.at("tcpReceiverDetails").is_object()){
                uint16_t tcpPortNumber;
                uint16_t tcptimeOut;
                if(config.at("tcpReceiverDetails").contains("portNumber")
                    && config.at("tcpReceiverDetails").at("portNumber").is_number()
                ){
                    tcpPortNumber = config.at("tcpReceiverDetails").at("portNumber");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded tcpPortNumber; value=["+std::to_string(tcpPortNumber)+"]");
                }
                if(config.at("tcpReceiverDetails").contains("timeOut")
                    && config.at("tcpReceiverDetails").at("timeOut").is_number()
                ){
                    tcptimeOut = config.at("tcpReceiverDetails").at("timeOut");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded tcptimeOut; value=["+std::to_string(tcptimeOut)+"]");
                }
                SharedDataStoreInstance->SetTcpReceiverDetails(tcpPortNumber, tcptimeOut);
            }
            return true;
        }
        
        void ConfigParser::SetDefaultConfigs()
        {
            auto SharedDataStoreInstance{
                std::dynamic_pointer_cast<systemcontext::SharedDataStore>(
                    systemcontext::ComponentRegistry::GetComponent("SharedDataStore")
                )
            };
            if(SharedDataStoreInstance == nullptr){
                logger::Logger::LOG("ConfigParser::SetDefaultConfigs", "Shared Data Store instance is not available", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            SharedDataStoreInstance->SetMaxEvent(10000);
            SharedDataStoreInstance->SetMemoryLimit("mb", 1024);
            SharedDataStoreInstance->SetUdpReceiverDetails(8080, 30);
            SharedDataStoreInstance->SetTcpReceiverDetails(9090, 30);
            logger::Logger::LOG("ConfigParser::SetDefaultConfigs", "Default configs set: maxEvents=10000, memoryType=mb, maxMemoryLimit=1024, udpPortNumber=8080, udptimeOut=30, tcpPortNumber=9090, tcptimeOut=30");
        }
    }
}
