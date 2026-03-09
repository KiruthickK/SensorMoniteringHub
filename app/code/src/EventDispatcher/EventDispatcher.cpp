#include "EventDispatcher/EventDispatcher.hpp"

namespace sensormoniteringhub{
    namespace eventdispatcher{
        void EventDispatcher::StartService()
        {
        }

        void EventDispatcher::StopService()
        {
        }

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
        void EventDispatcher::SetRegisteredComponents(std::map<std::string, std::shared_ptr<IEvents>> RegisteredComponents)
        {
            RegisteredComponents_ = RegisteredComponents;
        }
        void EventDispatcher::OnInitializeFinish(){
            logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Calling StartService for all components");
            for(auto const& itr : RegisteredComponents_){
                logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Calling StartService for component: " + itr.first, logger::LOGLEVEL::DEBUG_LEVEL);
                itr.second->StartService();
            }
            logger::Logger::LOG("EventDispatcher::OnInitializeFinish", "Completed Calling StartService for all components!");
        }
    }
}
