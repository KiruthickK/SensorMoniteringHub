# Sensor Monitoring Hub

A lightweight C++17 middleware service that collects data from motion detection sensors, processes them and persists it across sessions, and serves the information to ECUs (Electronic Control Units) on request.

---

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Build & Run](#build--run)
- [Debug Mode](#debug-mode)
- [Configuration](#configuration)
- [Data Persistence](#data-persistence)
- [Simulators](#simulators)
  - [UDP Sensor Data Simulator](#udp-sensor-data-simulator)
  - [TCP Request Client Simulator](#tcp-request-client-simulator)
  - [TCP Control Client Simulator](#tcp-control-client-simulator)
- [Prerequisites](#prerequisites)
- [Project Structure](#project-structure)

---

## Overview

The Sensor Monitoring Hub acts as a central broker between motion detection sensors and downstream ECUs. It handles:

- **Data Ingestion** — Receives raw sensor data over UDP
- **Processing & Storage** — Parses, validates, and stores incoming sensor readings in an in-memory buffer
- **Data Persistence** — Persists the in-memory buffer to a local SQLite database on shutdown and restores it on the next boot
- **Data Serving** — Responds to TCP-based ECU requests with filtered sensor information
- **Control Handling** — Accepts control commands over a separate TCP channel for config changes, data clearing, and graceful shutdown

---

## Architecture

The hub is composed of independent modules, each with a clearly defined responsibility:

| Module | Responsibility |
|--------|---------------|
| `SensorMonitoringHubManager` | Top-level lifecycle manager — starts and stops all services |
| `SensorDataReceiver` | Receives and validates raw UDP sensor payloads |
| `DataPool` | In-memory buffer for sensor data; enforces capacity limits |
| `PersistenceManager` | SQLite-backed persistence — persists buffer on shutdown, restores on boot |
| `ClientRequestService` | Handles TCP requests from ECUs; filters and encodes sensor data responses |
| `ControlCommandService` | Handles TCP control commands (clear data, config change, shutdown) |
| `NetworkInterfaceManager` | Manages UDP and TCP socket connections |
| `EventDispatcher` | Decoupled event bus between components |
| `TimerService` | Provides timer callbacks used across modules |
| `Logger` | Timestamped file-based logging |
| `JsonParser / ConfigParser` | Provides JSON parsing utilities used across all modules; `ConfigParser` specifically handles loading and reloading of `SMH_Config.json` at startup and on config change |

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

## Configuration

The hub reads its configuration from `app/config/SMH_Config.json` at startup. Key parameters include:

- **UDP port** — port on which sensor data is received
- **TCP ports** — separate ports for the request client and control client
- **Connection timeouts** — per-client timeout values
- **Max buffer size** — maximum in-memory storage for sensor data (in bytes); the event capacity is derived from this at runtime

On a **config change command** from the TCP control client, the hub restarts its services and reloads the config automatically.

---

## Data Persistence

The hub uses **SQLite** (bundled as an amalgamation — no installation required) to persist sensor data across sessions.

- The database file is stored at `app/data/smh.db`
- On **shutdown**, the in-memory buffer is flushed to the database in a single transaction
- On **boot**, the database is read back into the buffer (up to the configured capacity limit) and then cleared
- If the configured buffer size is reduced between runs, the newest records are prioritised on restore and excess records are discarded
- The database file is not committed to version control (see `.gitignore`)

No external database server or installation is required — SQLite is compiled directly into the binary from `app/external/libraries/SQLite/`.

---

## Simulators

Three simulators are provided to generate test data without requiring physical hardware.

### UDP Sensor Data Simulator

Simulates incoming sensor data over UDP. Run this to feed test readings into the hub.

```bash
cd simulators/udp_sensor_simulator
g++ -std=c++17 udp_sensor_simulation.cpp -o udpSim
./udpSim
```

Sample sensor data payloads are available in `simulators/udp_sensor_simulator/SensorDataSource/`.

### TCP Request Client Simulator

Simulates an ECU making TCP requests to the hub for sensor data.

```bash
cd simulators/tcp_request_client_simulator
g++ -std=c++17 TcpRequestClientSim.cpp -o tcpRequestClient
./tcpRequestClient
```

### TCP Control Client Simulator

Simulates a control client sending command orders to the hub over TCP. Supported commands:

- **Change Config** — Sends a new configuration to the hub; the hub restarts its services to apply the updated configuration
- **Clear Data** — Clears all currently stored sensor data in the hub
- **Shutdown** — Sends a shutdown command to gracefully stop the hub

```bash
cd simulators/tcp_control_client_simulator
g++ -std=c++17 TcpControlClientSim.cpp -o tcpControlClient
./tcpControlClient
```

---

## Prerequisites

| Tool | Version |
|------|---------|
| g++ / GCC | C++17 or later |
| GDB *(optional)* | Any recent version |
| SQLite | Bundled — no installation needed |

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
│   │   │   ├── ControlCommandService
│   │   │   ├── DataPool
│   │   │   ├── EventDispatcher
│   │   │   ├── Events
│   │   │   ├── JsonParser
│   │   │   ├── Logger
│   │   │   ├── NetworkInterfaceManager
│   │   │   ├── PersistenceManager
│   │   │   ├── SensorDataReceiver
│   │   │   ├── SensorMonitoringHubManager
│   │   │   ├── SystemContext
│   │   │   └── TimerService
│   │   └── src
│   │       ├── ClientRequestService
│   │       ├── ControlCommandService
│   │       ├── DataPool
│   │       ├── EventDispatcher
│   │       ├── JsonParser
│   │       ├── Logger
│   │       ├── NetworkInterfaceManager
│   │       ├── PersistenceManager
│   │       ├── SensorDataReceiver
│   │       ├── SensorMonitoringHubManager
│   │       ├── SystemContext
│   │       └── TimerService
│   ├── config
│   │   └── SMH_Config.json
│   ├── data
│   │   └── .gitkeep
│   ├── external
│   │   └── libraries
│   │       ├── JSON
│   │       │   └── nlohmann
│   │       └── SQLite
│   │           ├── sqlite3.c
│   │           └── sqlite3.h
│   ├── logs
│   └── tools
│       ├── build.sh
│       └── run.sh
├── README.md
└── simulators
    ├── tcp_control_client_simulator
    ├── tcp_request_client_simulator
    └── udp_sensor_simulator
        └── SensorDataSource
```