# Sensor Monitering Hub
Collects the data from motion detection sensors, process and stores them, and shares with ECU's which requests for the information

# Steps for Build and Run

cd app/tools
chmod 777 build.sh deploy.sh
./build.sh
./deploy.sh 

## if required to run with gdb
./deploy.sh -D

## steps for sending test data from simulators
### udp sensor data simulator
cd simulators/udp_sensor_simulation
g++ udp_sensor_simulation.cpp -o server
./server