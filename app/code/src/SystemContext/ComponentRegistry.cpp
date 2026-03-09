#include "SystemContext/ComponentRegistry.hpp"

namespace sensormoniteringhub{
    namespace systemcontext{
        std::shared_ptr<ComponentRegistry> ComponentRegistry::ComponentRegistryInstance_ = nullptr;
        void ComponentRegistry::StartService()
        {
            
        }

        void ComponentRegistry::StopService()
        {
        }

        void ComponentRegistry::Initialize()
        {
            ComponentRegistryInstance_ =
                std::make_shared<ComponentRegistry>();
            ComponentRegistryInstance_->RegisterComponent("ComponentRegistry", ComponentRegistryInstance_);
            std::cout<<"[ComponentRegistry::Initialize] Initialization successful!"<<std::endl;
        }
        void ComponentRegistry::Finalize()
        {
        }
        void ComponentRegistry::RegisterComponent(std::string componentName, std::shared_ptr<IEvents> componentInstance)
        {
            RegisteredComponents_.insert({componentName, componentInstance});
        }

        std::shared_ptr<IEvents> ComponentRegistry::GetComponent(std::string componentName)
        {
            if(ComponentRegistryInstance_->RegisteredComponents_.find(componentName) != ComponentRegistryInstance_->RegisteredComponents_.end()){
                return ComponentRegistryInstance_->RegisteredComponents_[componentName];
            }
            return {};
        }

        void ComponentRegistry::OnInitializeFinish(){
            logger::Logger::LOG("ComponentRegistry::OnInitializeFinish", "Sending registered components to EventDispatcher");
            std::dynamic_pointer_cast<eventdispatcher::EventDispatcher>(
                ComponentRegistry::GetComponent("EventDispatcher")
            )->SetRegisteredComponents(RegisteredComponents_);
            logger::Logger::LOG("ComponentRegistry::OnInitializeFinish", "Calling EventDispatcher::OnInitializeFinish");
            std::dynamic_pointer_cast<eventdispatcher::EventDispatcher>(
                ComponentRegistry::GetComponent("EventDispatcher")
            )->OnInitializeFinish();
        }
    }
}

