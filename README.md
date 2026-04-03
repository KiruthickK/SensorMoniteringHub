# Sensor Monitoring Hub

A lightweight middleware service that collects data from motion detection sensors, processes and persists it, and serves the information to ECUs (Electronic Control Units) on request.

---

## Table of Contents

- [Overview](#overview)
- [Build & Run](#build--run)
- [Debug Mode](#debug-mode)
- [Simulators](#simulators)
  - [UDP Sensor Data Simulator](#udp-sensor-data-simulator)
  - [TCP Request Client Simulator](#tcp-request-client-simulator)

---

## Overview

The Sensor Monitoring Hub acts as a central broker between motion detection sensors and downstream ECUs. It handles:

- **Data Ingestion** — Receives raw sensor data over UDP
- **Processing & Storage** — Parses, validates, and persists incoming sensor readings
- **Data Serving** — Responds to TCP-based ECU requests with the relevant sensor information

---

## Build & Run

Navigate to the tools directory and run the build and launch scripts:
```bash
cd app/tools
chmod +x build.sh run.sh
./build.sh
./run.sh
```

> **Note:** `chmod +x` is preferred over `chmod 777` to avoid granting unnecessary write permissions to all users.

---

## Debug Mode

To run the application with **GDB** attached for debugging:
```bash
./run.sh -D
```

---

## Simulators

Two simulators are provided to generate test data without requiring physical hardware.

### UDP Sensor Data Simulator

Simulates incoming sensor data over UDP. Run this to feed test readings into the hub.
```bash
cd simulators/udp_sensor_simulation
g++ -std=c++17 udp_sensor_simulation.cpp -o server
./server
```

### TCP Request Client Simulator

Simulates an ECU making TCP requests to the hub for sensor data.
```bash
cd simulators/tcp_request_client_simulator
g++ -std=c++17 TcpRequestClientSim.cpp -o tcpClient
./tcpClient
```

---

## Prerequisites

| Tool | Version |
|------|---------|
| g++ / GCC | C++17 or later |
| GDB *(optional)* | Any recent version |

---

## Project Structure
```
├── app
│   ├── cmake
│   │   ├── CMakeLists.txt
│   │   └── src_cmake
│   │       └── CMakeLists.txt
│   ├── code
│   │   ├── include
│   │   │   ├── ClientRequestService
│   │   │   │   ├── ClientRequestService.hpp
│   │   │   │   ├── IClientRequestHandler.hpp
│   │   │   │   ├── RequestData.hpp
│   │   │   │   ├── RequestParser.hpp
│   │   │   │   └── ResponseEncoder.hpp
│   │   │   ├── DataPool
│   │   │   │   └── DataPool.hpp
│   │   │   ├── EventDispatcher
│   │   │   │   └── EventDispatcher.hpp
│   │   │   ├── Events
│   │   │   │   └── IEvents.hpp
│   │   │   ├── JsonParser
│   │   │   │   ├── ConfigParser.hpp
│   │   │   │   └── JsonParser.hpp
│   │   │   ├── Logger
│   │   │   │   └── Logger.hpp
│   │   │   ├── NetworkInterfaceManager
│   │   │   │   ├── TCPReceiver.hpp
│   │   │   │   └── UDPReceiver.hpp
│   │   │   ├── SensorDataReceiver
│   │   │   │   ├── SensorData.hpp
│   │   │   │   └── SensorDataReceiver.hpp
│   │   │   ├── SensorMonitoringHubManager
│   │   │   │   └── SensorMonitoringHubManager.hpp
│   │   │   ├── SystemContext
│   │   │   │   ├── ComponentRegistry.hpp
│   │   │   │   └── SharedDataStore.hpp
│   │   │   └── TimerService
│   │   │       └── TimerService.hpp
│   │   └── src
│   │       ├── ClientRequestService
│   │       │   ├── ClientRequestService.cpp
│   │       │   ├── RequestParser.cpp
│   │       │   └── ResponseEncoder.cpp
│   │       ├── DataPool
│   │       │   └── DataPool.cpp
│   │       ├── EventDispatcher
│   │       │   └── EventDispatcher.cpp
│   │       ├── JsonParser
│   │       │   ├── ConfigParser.cpp
│   │       │   └── JsonParser.cpp
│   │       ├── Logger
│   │       │   └── Logger.cpp
│   │       ├── NetworkInterfaceManager
│   │       │   ├── TCPReceiver.cpp
│   │       │   └── UDPReceiver.cpp
│   │       ├── SensorDataReceiver
│   │       │   └── SensorDataReceiver.cpp
│   │       ├── SensorMonitoringHubManager
│   │       │   └── SensorMonitoringHubManager.cpp
│   │       ├── SystemContext
│   │       │   ├── ComponentRegistry.cpp
│   │       │   └── SharedDataStore.cpp
│   │       └── TimerService
│   │           └── TimerService.cpp
│   ├── config
│   │   └── SMH_Config.json
│   ├── external
│   │   └── libraries
│   │       └── JSON
│   │           └── nlohmann
│   │               ├── json_fwd.hpp
│   │               └── json.hpp
│   ├── logs
│   │   ├── Log-Mar-24-10Hrs-33Mins.txt
│   │   ├── Log-Mar-24-10Hrs-53Mins.txt
│   │   └── Log-Mar-24-10Hrs-57Mins.txt
│   └── tools
│       ├── build.sh
│       └── run.sh
├── README.md
└── simulators
    ├── nlohmann
    │   ├── json_fwd.hpp
    │   └── json.hpp
    ├── tcp_request_client_simulator
    │   ├── tcpClient
    │   └── TcpRequestClientSim.cpp
    └── udp_sensor_simulator
        ├── SensorDataSource
        │   ├── invalid_data_1.json
        │   ├── valid_data_1.json
        │   ├── valid_data_2.json
        │   ├── valid_data_3.json
        │   └── valid_data_4.json
        └── udp_sensor_simulation.cpp

41 directories, 55 files
```