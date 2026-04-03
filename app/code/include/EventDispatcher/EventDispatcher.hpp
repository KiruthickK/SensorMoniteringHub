#ifndef EVENTDISPATCHER_HEADER
#define EVENTDISPATCHER_HEADER

#include <Events/IEvents.hpp>
#include <map>
#include <Logger/Logger.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SensorMonitoringHubManager/SensorMonitoringHubManager.hpp>

#include <memory>
namespace sensormoniteringhub{
    namespace eventdispatcher{
        class EventDispatcher : public IEvents{
            private:
            std::map<std::string, std::shared_ptr<IEvents>> RegisteredComponents_;
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            void SetRegisteredComponents(std::map<std::string, std::shared_ptr<IEvents>> RegisteredComponents);
            void OnInitializeFinish();
            bool OnClearEvents();
        };
    }
}

#endif