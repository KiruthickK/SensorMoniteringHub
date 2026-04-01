#include <JsonParser/ConfigParser.hpp>

namespace sensormoniteringhub{
    namespace jsonparser{
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
            }else{
                SharedDataStoreInstance->SetMaxEvent(10000); // setting default value in case of failure
            }
            if(config.contains("memoryLimit") && config.at("memoryLimit").is_object()){
                std::string memoryType{""};
                uint16_t maxMemoryLimit{0U};
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
                if(!memoryType.empty() && maxMemoryLimit != 0U){
                    SharedDataStoreInstance->SetMemoryLimit(memoryType, maxMemoryLimit);
                }else{
                    SharedDataStoreInstance->SetMemoryLimit("mb", 1024); // setting default value in case of failure
                }
            }
            if(config.contains("udpReceiverDetails") && config.at("udpReceiverDetails").is_object()){
                uint16_t udpPortNumber{0U};
                uint16_t udptimeOut{0U};
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
                if(udpPortNumber != 0U && udptimeOut != 0U){
                    SharedDataStoreInstance->SetUdpReceiverDetails(udpPortNumber, udptimeOut);
                }else{
                    SharedDataStoreInstance->SetUdpReceiverDetails(8080, 30); // setting default value in case of failure
                }
            }
            if(config.contains("tcpOrderReceiverDetails") && config.at("tcpOrderReceiverDetails").is_object()){
                uint16_t tcpPortNumber{0U};
                uint16_t tcptimeOut{0U};
                if(config.at("tcpOrderReceiverDetails").contains("portNumber")
                    && config.at("tcpOrderReceiverDetails").at("portNumber").is_number()
                ){
                    tcpPortNumber = config.at("tcpOrderReceiverDetails").at("portNumber");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded tcpPortNumber; value=["+std::to_string(tcpPortNumber)+"]");
                }
                if(config.at("tcpOrderReceiverDetails").contains("timeOut")
                    && config.at("tcpOrderReceiverDetails").at("timeOut").is_number()
                ){
                    tcptimeOut = config.at("tcpOrderReceiverDetails").at("timeOut");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded tcptimeOut; value=["+std::to_string(tcptimeOut)+"]");
                }
                if(tcpPortNumber != 0U && tcptimeOut != 0U){
                    SharedDataStoreInstance->SetTcpOrderReceiverDetails(tcpPortNumber, tcptimeOut);
                }else{
                    SharedDataStoreInstance->SetTcpOrderReceiverDetails(8090, 30); // setting default value in case of failure
                }
            }
            if(config.contains("tcpClientRequestServiceDetails") && config.at("tcpClientRequestServiceDetails").is_object()){
                uint16_t tcpPortNumber{0U};
                uint16_t tcptimeOut{0U};
                if(config.at("tcpClientRequestServiceDetails").contains("portNumber")
                    && config.at("tcpClientRequestServiceDetails").at("portNumber").is_number()
                ){
                    tcpPortNumber = config.at("tcpClientRequestServiceDetails").at("portNumber");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded tcpPortNumber; value=["+std::to_string(tcpPortNumber)+"]");
                }
                if(config.at("tcpClientRequestServiceDetails").contains("timeOut")
                    && config.at("tcpClientRequestServiceDetails").at("timeOut").is_number()
                ){
                    tcptimeOut = config.at("tcpClientRequestServiceDetails").at("timeOut");
                    logger::Logger::LOG("ConfigParser::LoadConfigs", "Loaded tcptimeOut; value=["+std::to_string(tcptimeOut)+"]");
                }
                if(tcpPortNumber != 0U && tcptimeOut != 0U){
                    SharedDataStoreInstance->SetTcpClientRequestServiceDetails(tcpPortNumber, tcptimeOut);
                }else{
                    SharedDataStoreInstance->SetTcpClientRequestServiceDetails(9090, 30); // setting default value in case of failure
                }
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
            SharedDataStoreInstance->SetTcpOrderReceiverDetails(8090, 30);
            SharedDataStoreInstance->SetTcpClientRequestServiceDetails(9090, 30);
            logger::Logger::LOG("ConfigParser::SetDefaultConfigs", "Default configs set: maxEvents=10000, memoryType=mb, maxMemoryLimit=1024, udpPortNumber=8080, udptimeOut=30, tcpPortNumber=9090, tcptimeOut=30");
        }
    }
}
