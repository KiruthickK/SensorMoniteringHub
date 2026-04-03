#include <SensorMonitoringHubManager/SensorMonitoringHubManager.hpp>
void SignalHandler(int signal);
namespace sensormoniteringhub{
    /// @brief for tracking the finalization
    std::atomic<bool> gShutdownRequested{false};
    namespace sensormonitoringhubmanager{
        std::atomic<bool> SensorMonitoringHubManager::restartFlag_;
        /// @brief Starts the Sensor Monitoring Hub service by loading configurations and signaling the completion of initialization.
        void SensorMonitoringHubManager::StartService()
        {
            restartFlag_.store(false);
            if(jsonparser::ConfigParser::LoadConfigs(configPath)){
                logger::Logger::LOG("SensorMonitoringHubManager::StartService", "Configs Loaded Successfully");
            } else {
                logger::Logger::LOG("SensorMonitoringHubManager::StartService", "Failed to Load Configs; setting default configs", logger::LOGLEVEL::WARNING_LEVEL);
                jsonparser::ConfigParser::SetDefaultConfigs();
            }
            logger::Logger::LOG("SensorMonitoringHubManager::StartService", "Calling start service event");
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->OnInitializeFinish();
        }

        /// @brief method for triggering clear events
        /// @return true if success
        bool SensorMonitoringHubManager::ClearEvents(){
            auto DataPoolInstance{
                std::dynamic_pointer_cast<datapool::DataPool>(
                    systemcontext::ComponentRegistry::GetComponent("DataPool")
                )
            };
            if(!DataPoolInstance){
                logger::Logger::LOG("SensorMonitoringHubManager::ClearEvents", "DataPool instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            logger::Logger::LOG("SensorMonitoringHubManager::ClearEvents", "Calling DataPool for Clearing the Events");
            return DataPoolInstance->ClearEvents();
        }

        /// @brief method for triggering stopservice on all the modules
        void SensorMonitoringHubManager::StopService()
        {
            auto eventDispatcherInstance{
                std::dynamic_pointer_cast<eventdispatcher::EventDispatcher>(
                    systemcontext::ComponentRegistry::GetComponent("EventDispatcher")
                )
            };
            if(!eventDispatcherInstance){
                logger::Logger::LOG("SensorMonitoringHubManager::StopService", "Event Dispatcher instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return;
            }
            logger::Logger::LOG("SensorMonitoringHubManager::StopService", "Calling Stop service Event!");
            eventDispatcherInstance->StopService();
        }

        /// @brief Initializes all necessary components for the Sensor Monitoring Hub Manager.
        void SensorMonitoringHubManager::Initialize(std::shared_ptr<SensorMonitoringHubManager> SMHInstance)
        {
            systemcontext::ComponentRegistry::Initialize();
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("SensorMonitoringHubManager", SMHInstance);
            logger::Logger::Initialize();
            systemcontext::SharedDataStore::Initialize();
            eventdispatcher::EventDispatcher::Initialize();
            networkinterfacemanager::TCPReceiver::Initialize();
            networkinterfacemanager::UDPReceiver::Initialize();
            sensordatareceiver::SensorDataReceiver::Initialize();
            datapool::DataPool::Initialize();
            jsonparser::JsonParser::Initialize();
            jsonparser::ConfigParser::Initialize();
            clientrequestservice::ClientRequestService::Initialize();
            clientrequestservice::RequestParser::Initialize();
            clientrequestservice::ResponseEncoder::Initialize();
            controlcommandservice::ControlCommandService::Initialize();
            controlcommandservice::CommandParser::Initialize();
            controlcommandservice::CommandResponseEncoder::Initialize();
            timerservice::TimerService::Initialize();
            logger::Logger::LOG("SensorMonitoringHubManager::Initialize", "All components Initialized");
        }

        /// @brief method for calling finalize on all the modules
        void SensorMonitoringHubManager::Finalize()
        {
            std::dynamic_pointer_cast<eventdispatcher::EventDispatcher>(
                systemcontext::ComponentRegistry::GetComponent("EventDispatcher")
            )->StopService();
            timerservice::TimerService::Finalize();
            clientrequestservice::ResponseEncoder::Finalize();
            clientrequestservice::RequestParser::Finalize();
            clientrequestservice::ClientRequestService::Finalize();
            controlcommandservice::CommandResponseEncoder::Finalize();
            controlcommandservice::CommandParser::Finalize();    
            controlcommandservice::ControlCommandService::Finalize();
            jsonparser::ConfigParser::Finalize();
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

        /// @brief method for triggering config file change and triggering restarting services to apply new config data
        /// @param configData 
        /// @return 
        bool SensorMonitoringHubManager::ChangeConfig(std::string const& configData){
            auto ConfigParserInstance{
                std::dynamic_pointer_cast<jsonparser::ConfigParser>(
                    systemcontext::ComponentRegistry::GetComponent("ConfigParser")
                )
            };
            if(!ConfigParserInstance){
                logger::Logger::LOG("SensorMonitoringHubManager::ChangeConfig", "ConfigParser instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            auto eventDispatcherInstance{
                std::dynamic_pointer_cast<eventdispatcher::EventDispatcher>(
                    systemcontext::ComponentRegistry::GetComponent("EventDispatcher")
                )
            };
            if(!eventDispatcherInstance){
                logger::Logger::LOG("SensorMonitoringHubManager::ChangeConfig", "Event Dispatcher instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            logger::Logger::LOG("SensorMonitoringHubManager::ChangeConfig", "Registering Restart services method to Event Dispatcher!");
            eventDispatcherInstance->RegisterPostResponseCallback([this](){
                logger::Logger::LOG("SensorMonitoringHubManager::ChangeConfig", "About to restart services to apply new config File!", logger::LOGLEVEL::LIFECYCLE_LEVEL);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                RestartServices();
            });
            logger::Logger::LOG("SensorMonitoringHubManager::ChangeConfig", "Calling ConfigParser Instance for changing the config files");
            return ConfigParserInstance->ChangeConfigFile(configData);
        }

        /// @brief method for restarting services
        void SensorMonitoringHubManager::RestartServices(){
            logger::Logger::LOG("SensorMonitoringHubManager::RestartServices", "Restarting services to apply new config File!", logger::LOGLEVEL::LIFECYCLE_LEVEL);
            StopService();
            logger::Logger::LOG("SensorMonitoringHubManager::RestartServices", "All services stopped, starting again!", logger::LOGLEVEL::LIFECYCLE_LEVEL);
            StartService();
            logger::Logger::LOG("SensorMonitoringHubManager::RestartServices", "Services restarted and new config applied successfully!", logger::LOGLEVEL::LIFECYCLE_LEVEL);
        }

        /// @brief method for registering ShutDownTrigger callback to EventDispatcher
        /// @return true on success
        bool SensorMonitoringHubManager::RegisterShutDownTriggerToEventDispatcher(){
            auto eventDispatcherInstance{
                std::dynamic_pointer_cast<eventdispatcher::EventDispatcher>(
                    systemcontext::ComponentRegistry::GetComponent("EventDispatcher")
                )
            };
            if(!eventDispatcherInstance){
                logger::Logger::LOG("SensorMonitoringHubManager::RegisterShutDownTriggerToEventDispatcher", "Event Dispatcher instance not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            logger::Logger::LOG("SensorMonitoringHubManager::RegisterShutDownTriggerToEventDispatcher", "Registering ShutDownTrigger method to Event Dispatcher!");
            eventDispatcherInstance->RegisterPostResponseCallback([this](){
                logger::Logger::LOG("SensorMonitoringHubManager::RegisterShutDownTriggerToEventDispatcher", "Calling ShutDownTrigger!", logger::LOGLEVEL::LIFECYCLE_LEVEL);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                ShutDownTrigger();
            });
            return true;
        }

        /// @brief method for triggering shutdown
        void SensorMonitoringHubManager::ShutDownTrigger(){
            logger::Logger::LOG("SensorMonitoringHubManager::ShutDownTrigger", "Application is about to start shutdown!", logger::LOGLEVEL::LIFECYCLE_LEVEL);
            SignalHandler(SIGTERM);
        }

        /// @brief method for triggering RegisterShutDownTriggerToEventDispatcher
        /// @return true on success
        bool SensorMonitoringHubManager::ShutDownRequest(){
            return RegisterShutDownTriggerToEventDispatcher();
        }
    }
}

/// @brief method for handling SIGTERM and SIGINT signals
/// @param signal 
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
    std::shared_ptr<sensormoniteringhub::sensormonitoringhubmanager::SensorMonitoringHubManager> SensorMoniteringHub_{
        std::make_shared<sensormoniteringhub::sensormonitoringhubmanager::SensorMonitoringHubManager>()
    };
    SensorMoniteringHub_->Initialize(SensorMoniteringHub_);
    SensorMoniteringHub_->StartService();
    while(!sensormoniteringhub::gShutdownRequested.load()){
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    SensorMoniteringHub_->Finalize();
    return 0;
}
