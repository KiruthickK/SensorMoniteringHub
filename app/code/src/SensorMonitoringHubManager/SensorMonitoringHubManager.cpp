#include <SensorMonitoringHubManager/SensorMonitoringHubManager.hpp>

namespace sensormoniteringhub{
    namespace sensormonitoringhubmanager{

        /// @brief Starts the Sensor Monitoring Hub service by loading configurations and signaling the completion of initialization.
        void SensorMonitoringHubManager::StartService()
        {
            if(jsonparser::ConfigParser::LoadConfigs(configPath)){
                logger::Logger::LOG("SensorMonitoringHubManager::StartService", "Configs Loaded Successfully");
            } else {
                logger::Logger::LOG("SensorMonitoringHubManager::StartService", "Failed to Load Configs; setting default configs", logger::LOGLEVEL::WARNING_LEVEL);
                jsonparser::ConfigParser::SetDefaultConfigs();
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
            std::dynamic_pointer_cast<eventdispatcher::EventDispatcher>(
                systemcontext::ComponentRegistry::GetComponent("EventDispatcher")
            )->StopService();
            timerservice::TimerService::Finalize();
            clientrequestservice::ResponseEncoder::Finalize();
            clientrequestservice::RequestParser::Finalize();
            clientrequestservice::NotificationSender::Finalize();
            jsonparser::JsonParser::Finalize();
            datapool::DataPool::Finalize();
            sensordatareceiver::SensorDataReceiver::Finalize();
            networkinterfacemanager::UDPReceiver::Finalize();
            networkinterfacemanager::TCPReceiver::Finalize();
            eventdispatcher::EventDispatcher::Finalize();
            systemcontext::SharedDataStore::Finalize();
            logger::Logger::Finalize();
            systemcontext::ComponentRegistry::Finalize();
        }
    }
}
void SignalHandler(int signal)
{
    if(signal == SIGTERM || signal == SIGINT)
    {
        sensormoniteringhub::gShutdownRequested.store(true);
    }
}
/// @brief The main entry point for the Sensor Monitoring Hub Manager application.
/// @param argc The number of command-line arguments.
/// @param argv An array of command-line argument strings.
/// @return The exit status of the application.
int main(int argc, char const *argv[])
{
    // prctl(PR_SET_NAME, "SMH", 0, 0, 0);
    std::signal(SIGTERM, SignalHandler);
    std::signal(SIGINT, SignalHandler);
    std::unique_ptr<sensormoniteringhub::sensormonitoringhubmanager::SensorMonitoringHubManager> SensorMoniteringHub_{
        std::make_unique<sensormoniteringhub::sensormonitoringhubmanager::SensorMonitoringHubManager>()
    };
    SensorMoniteringHub_->Initialize();
    SensorMoniteringHub_->StartService();
    while(!sensormoniteringhub::gShutdownRequested.load()){
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    SensorMoniteringHub_->Finalize();
    return 0;
}
