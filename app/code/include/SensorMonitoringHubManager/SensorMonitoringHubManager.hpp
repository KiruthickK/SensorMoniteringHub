#ifndef SENSOR_MONITERING_HUB_MANAGER_HEADER
#define SENSOR_MONITERING_HUB_MANAGER_HEADER

#include <iostream>
#include <memory>
#include <Events/IEvents.hpp>
#include <SensorMonitoringHubManager/ConfigParser.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>
#include <Logger/Logger.hpp>
#include <EventDispatcher/EventDispatcher.hpp>
#include <NetworkInterfaceManager/TCPReceiver.hpp>
#include <NetworkInterfaceManager/UDPReceiver.hpp>
#include <SensorDataReceiver/SensorDataReceiver.hpp>
#include <DataPool/DataPool.hpp>
#include <JsonParser/JsonParser.hpp>
#include <ClientRequestService/NotificationSender.hpp>
#include <ClientRequestService/RequestParser.hpp>
#include <ClientRequestService/ResponseEncoder.hpp>
#include <TimerService/TimerService.hpp>

namespace sensormoniteringhub{
    namespace sensormonitoringhubmanager{
        class SensorMonitoringHubManager : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
        };
    }
}

#endif