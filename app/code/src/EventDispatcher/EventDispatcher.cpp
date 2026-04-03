#include "EventDispatcher/EventDispatcher.hpp"

namespace sensormoniteringhub{
    namespace eventdispatcher{
        void EventDispatcher::StartService()
        {
        }

        void EventDispatcher::StopService()
        {
            logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Calling StopService for all components");
            for(auto const& itr : RegisteredComponents_){
                if(itr.first == "EventDispatcher" || itr.first == "Logger" || itr.first == "SensorMonitoringHubManager") continue;
                logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Calling StopService for component: " + itr.first, logger::LOGLEVEL::DEBUG_LEVEL);
                itr.second->StopService();
            }
            logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Completed Calling StopService for all components!");
            RegisteredComponents_["Logger"]->StopService();
        }

        /// @brief Initializes the EventDispatcher by creating an instance and registering itself as a component.
        void EventDispatcher::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("EventDispatcher", std::make_shared<EventDispatcher>());
            logger::Logger::LOG("EventDispatcher::Initialize", "Initialization successful!");
        }

        void EventDispatcher::Finalize()
        {
        }

        /// @brief Sets the map of registered components for the EventDispatcher.
        /// @param RegisteredComponents The map of registered components.
        void EventDispatcher::SetRegisteredComponents(std::map<std::string, std::shared_ptr<IEvents>> RegisteredComponents)
        {
            RegisteredComponents_ = RegisteredComponents;
        }

        /// @brief Called when the initialization of all components is finished, signaling the EventDispatcher to proceed with its initialization.
        void EventDispatcher::OnInitializeFinish(){
            logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Calling StartService for all components");
            for(auto const& itr : RegisteredComponents_){
                // Avoid re-triggering orchestrator components and causing recursive initialization calls
                if(itr.first == "SensorMonitoringHubManager"){
                    logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Skipping StartService for orchestrator component: " + itr.first, logger::LOGLEVEL::DEBUG_LEVEL);
                    continue;
                }
                logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Calling StartService for component: " + itr.first, logger::LOGLEVEL::DEBUG_LEVEL);
                itr.second->StartService();
            }
            logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Completed Calling StartService for all components!");
        }

        /// @brief 
        /// @return 
        bool EventDispatcher::OnClearEvents(){
            logger::Logger::LOG("EventDispatcher::OnClearEvents", "Triggering Clear Events");
            auto SMHInstance{
                std::dynamic_pointer_cast<sensormonitoringhubmanager::SensorMonitoringHubManager>(systemcontext::ComponentRegistry::GetComponent("SensorMonitoringHubManager"))
            };
            if(!SMHInstance){
                logger::Logger::LOG("EventDispatcher::OnClearEvents", "Sensor Monitering Hub Instance Not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            return SMHInstance->ClearEvents();
        }

        /// @brief method for changing the config file
        /// @param newConfigData 
        /// @return true if config is changed
        bool EventDispatcher::OnConfigChange(std::string const& newConfigData){
            logger::Logger::LOG("EventDispatcher::OnConfigChange", "Triggering Config Change");
            auto SMHInstance{
                std::dynamic_pointer_cast<sensormonitoringhubmanager::SensorMonitoringHubManager>(systemcontext::ComponentRegistry::GetComponent("SensorMonitoringHubManager"))
            };
            if(!SMHInstance){
                logger::Logger::LOG("EventDispatcher::OnClearEvents", "Sensor Monitering Hub Instance Not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            return SMHInstance->ChangeConfig(newConfigData);
        }
        /// @brief method for triggering application shutdown
        /// @return true if request can be executed
        bool EventDispatcher::OnShutDownRequest(){
            logger::Logger::LOG("EventDispatcher::OnShutDownRequest", "Triggering Config Change");
            auto SMHInstance{
                std::dynamic_pointer_cast<sensormonitoringhubmanager::SensorMonitoringHubManager>(systemcontext::ComponentRegistry::GetComponent("SensorMonitoringHubManager"))
            };
            if(!SMHInstance){
                logger::Logger::LOG("EventDispatcher::OnClearEvents", "Sensor Monitering Hub Instance Not available!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            return SMHInstance->ShutDownRequest();
        }

        /// @brief method for registering the call back
        /// @param callback 
        void EventDispatcher::RegisterPostResponseCallback(std::function<void()> callback){
            logger::Logger::LOG("EventDispatcher::RegisterPostResponseCallback", "callback registered!");
            postResponseCallback_ = callback;
        }
        /// @brief method for triggering the callback
        void EventDispatcher::TriggerPostResponseCallback(){
            if(!postResponseCallback_){
                logger::Logger::LOG("EventDispatcher::TriggerPostResponseCallback", "No callback registered!", logger::LOGLEVEL::DEBUG_LEVEL);
                return;
            }
            if(!sensormonitoringhubmanager::SensorMonitoringHubManager::restartFlag_){
                sensormonitoringhubmanager::SensorMonitoringHubManager::restartFlag_.store(true);
                auto prCb = std::move(postResponseCallback_);
                postResponseCallback_ = nullptr;
                std::thread([prCb](){
                    try{
                        prCb();
                    } catch(std::exception const& e) {
                        logger::Logger::LOG("EventDispatcher::TriggerPostResponseCallback", e.what(), logger::LOGLEVEL::ERROR_LEVEL);
                    } catch(...) {
                        logger::Logger::LOG("EventDispatcher::TriggerPostResponseCallback", "Unknown exception", logger::LOGLEVEL::ERROR_LEVEL);
                    }
                }).detach();
            }else{
                logger::Logger::LOG("EventDispatcher::TriggerPostResponseCallback", "Duplicate Restart trigger!", logger::LOGLEVEL::ERROR_LEVEL);
            }
        }
    }
}
