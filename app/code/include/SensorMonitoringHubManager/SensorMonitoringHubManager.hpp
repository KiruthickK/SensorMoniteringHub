#ifndef SENSOR_MONITERING_HUB_MANAGER_HEADER
#define SENSOR_MONITERING_HUB_MANAGER_HEADER

#include <iostream>
#include <memory>
#include <thread>
#include <sys/prctl.h>
#include <csignal>
#include <Events/IEvents.hpp>
#include <JsonParser/ConfigParser.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>
#include <Logger/Logger.hpp>
#include <EventDispatcher/EventDispatcher.hpp>
#include <NetworkInterfaceManager/TCPReceiver.hpp>
#include <NetworkInterfaceManager/UDPReceiver.hpp>
#include <SensorDataReceiver/SensorDataReceiver.hpp>
#include <DataPool/DataPool.hpp>
#include <JsonParser/JsonParser.hpp>
#include <ClientRequestService/ClientRequestService.hpp>
#include <ClientRequestService/RequestParser.hpp>
#include <ClientRequestService/ResponseEncoder.hpp>
#include <ControlCommandService/CommandParser.hpp>
#include <ControlCommandService/CommandResponseEncoder.hpp>
#include <ControlCommandService/ControlCommandService.hpp>
#include <TimerService/TimerService.hpp>

namespace sensormoniteringhub{
    namespace sensormonitoringhubmanager{
        /// @brief SensorMonitoringHubManager class responsible for managing the Sensor Monitoring Hub service, including initialization, configuration loading, and service lifecycle management.
        class SensorMonitoringHubManager : public IEvents{
            private:
            /// @brief The file path to the JSON configuration file for the Sensor Monitoring Hub Manager.
            std::string const configPath{"../config/SMH_Config.json"};
            
            public:
            /// @brief Starts the Sensor Monitoring Hub service by loading configurations and signaling the completion of initialization.
            virtual void StartService();
            /// @brief Stops the Sensor Monitoring Hub service.
            virtual void StopService();
            /// @brief Initializes all necessary components for the Sensor Monitoring Hub Manager.
            static void Initialize(std::shared_ptr<SensorMonitoringHubManager> SMHInstance);
            /// @brief Finalizes the Sensor Monitoring Hub Manager, performing any necessary cleanup.
            static void Finalize();
            /// @brief Method for triggering clear events
            bool ClearEvents();
        };
    }
}

#endif