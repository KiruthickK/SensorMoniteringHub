#include <SensorMonitoringHubManager/SensorMonitoringHubManager.hpp>

namespace sensormoniteringhub{
    namespace sensormonitoringhubmanager{

        /// @brief Starts the Sensor Monitoring Hub service by loading configurations and signaling the completion of initialization.
        void SensorMonitoringHubManager::StartService()
        {
            if(ConfigParser::LoadConfigs(configPath)){
                logger::Logger::LOG("SensorMonitoringHubManager::StartService", "Configs Loaded Successfully");
            } else {
                logger::Logger::LOG("SensorMonitoringHubManager::StartService", "Failed to Load Configs; setting default configs", logger::LOGLEVEL::WARNING_LEVEL);
                ConfigParser::SetDefaultConfigs();
            }
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->OnInitializeFinish();
        }

        void SensorMonitoringHubManager::StopService()
        {
        }

        /// @brief Initializes all necessary components for the Sensor Monitoring Hub Manager.
        void SensorMonitoringHubManager::Initialize()
        {
            systemcontext::ComponentRegistry::Initialize();
            logger::Logger::Initialize();
            systemcontext::SharedDataStore::Initialize();
            eventdispatcher::EventDispatcher::Initialize();
            networkinterfacemanager::TCPReceiver::Initialize();
            networkinterfacemanager::UDPReceiver::Initialize();
            sensordatareceiver::SensorDataReceiver::Initialize();
            datapool::DataPool::Initialize();
            jsonparser::JsonParser::Initialize();
            clientrequestservice::NotificationSender::Initialize();
            clientrequestservice::RequestParser::Initialize();
            clientrequestservice::ResponseEncoder::Initialize();
            timerservice::TimerService::Initialize();
            logger::Logger::LOG("SensorMonitoringHubManager::Initialize", "All components Initialized");
        }
        void SensorMonitoringHubManager::Finalize()
        {
        }
    }
}

/// @brief The main entry point for the Sensor Monitoring Hub Manager application.
/// @param argc The number of command-line arguments.
/// @param argv An array of command-line argument strings.
/// @return The exit status of the application.
int main(int argc, char const *argv[])
{
    std::unique_ptr<sensormoniteringhub::sensormonitoringhubmanager::SensorMonitoringHubManager> SensorMoniteringHub_{
        std::make_unique<sensormoniteringhub::sensormonitoringhubmanager::SensorMonitoringHubManager>()
    };
    SensorMoniteringHub_->Initialize();
    SensorMoniteringHub_->StartService();
    return 0;
}
