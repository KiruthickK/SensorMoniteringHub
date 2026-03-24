#ifndef SENSOR_DATA_HEADER
#define SENSOR_DATA_HEADER

#include <string>
#include <stdint.h>

namespace sensormoniteringhub {
    namespace sensordatareceiver {
        struct SensorData{
            std::string sensorId_;
            uint64_t timeStamp_;
            double motionIntensity_;
            double temperature_;
            uint8_t batteryLevel_;
            std::string zoneId_;
            SensorData(): sensorId_(""), timeStamp_(0U), motionIntensity_(0), temperature_(0), batteryLevel_(0), zoneId_(""){}
            SensorData(std::string sensorId, uint64_t timeStamp, double motionIntensity, double temperature, uint8_t batteryLevel, std::string zoneId) 
                : sensorId_(sensorId), timeStamp_(timeStamp), motionIntensity_(motionIntensity), temperature_(temperature), batteryLevel_(batteryLevel), zoneId_(zoneId){}
        };
    }
}

#endif