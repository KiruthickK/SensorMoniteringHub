#include <SensorMonitoringHubManager/SensorMonitoringHubManager.hpp>

namespace sensormoniteringhub{
    namespace sensormonitoringhubmanager{
        void SensorMonitoringHubManager::StartService()
        {

        }

        void SensorMonitoringHubManager::StopService()
        {
        }
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
            ConfigParser::LoadConfigs();
            logger::Logger::LOG("SensorMonitoringHubManager::Initialize", "All components Initialized");
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->OnInitializeFinish();
        }
        void SensorMonitoringHubManager::Finalize()
        {
        }
    }
}


int main(int argc, char const *argv[])
{
    std::unique_ptr<sensormoniteringhub::sensormonitoringhubmanager::SensorMonitoringHubManager> SensorMoniteringHub_{
        std::make_unique<sensormoniteringhub::sensormonitoringhubmanager::SensorMonitoringHubManager>()
    };
    SensorMoniteringHub_->Initialize();
    SensorMoniteringHub_->StartService();
    return 0;
}
