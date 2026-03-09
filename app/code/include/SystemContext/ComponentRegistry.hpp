#ifndef COMPONENT_REGISTRY_HEADER
#define COMPONENT_REGISTRY_HEADER
#include <Events/IEvents.hpp>
#include <EventDispatcher/EventDispatcher.hpp>
#include <iostream>
#include <memory>
#include <map>

namespace sensormoniteringhub{
    namespace systemcontext{
        class ComponentRegistry : public IEvents{
            private:
            std::map<std::string, std::shared_ptr<IEvents>> RegisteredComponents_;
            static std::shared_ptr<ComponentRegistry> ComponentRegistryInstance_;
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            static std::shared_ptr<IEvents> GetComponent(std::string componentName);
            void RegisterComponent(std::string, std::shared_ptr<IEvents>);
            void OnInitializeFinish();
        };
    }
}

#endif