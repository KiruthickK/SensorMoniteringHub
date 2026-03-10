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

        /// @brief Initializes the ComponentRegistry by creating an instance and registering itself as a component.
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

        /// @brief Registers a component with the ComponentRegistry.
        /// @param componentName The name of the component to register.
        /// @param componentInstance A shared pointer to the component instance.
        void ComponentRegistry::RegisterComponent(std::string componentName, std::shared_ptr<IEvents> componentInstance)
        {
            RegisteredComponents_.insert({componentName, componentInstance});
        }

        /// @brief Retrieves a registered component by its name.
        /// @param componentName The name of the component to retrieve.
        /// @return A shared pointer to the requested component, or a null pointer if not found.
        std::shared_ptr<IEvents> ComponentRegistry::GetComponent(std::string componentName)
        {
            if(ComponentRegistryInstance_->RegisteredComponents_.find(componentName) != ComponentRegistryInstance_->RegisteredComponents_.end()){
                return ComponentRegistryInstance_->RegisteredComponents_[componentName];
            }
            return {};
        }

        /// @brief Called when the initialization of all components is finished, signaling the EventDispatcher to proceed with its initialization.
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

