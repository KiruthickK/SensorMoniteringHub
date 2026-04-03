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
                if(itr.first == "EventDispatcher" || itr.first == "Logger") continue;
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
    }
}
