#ifndef EVENTDISPATCHER_HEADER
#define EVENTDISPATCHER_HEADER

#include <Events/IEvents.hpp>
#include <Logger/Logger.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SensorMonitoringHubManager/SensorMonitoringHubManager.hpp>
#include <map>
#include <functional>
#include <thread>

#include <memory>
namespace sensormoniteringhub{
    namespace eventdispatcher{
        class EventDispatcher : public IEvents{
            private:
            std::map<std::string, std::shared_ptr<IEvents>> RegisteredComponents_;
            std::function<void()> postResponseCallback_;
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            void SetRegisteredComponents(std::map<std::string, std::shared_ptr<IEvents>> RegisteredComponents);
            void OnInitializeFinish();
            bool OnClearEvents();
            bool OnConfigChange(std::string const& newConfigData);
            bool OnShutDownRequest();
            void RegisterPostResponseCallback(std::function<void()> callback);
            void TriggerPostResponseCallback();
        };
    }
}

#endif