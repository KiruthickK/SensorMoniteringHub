#include "JsonParser/JsonParser.hpp"

namespace sensormoniteringhub{
    namespace jsonparser{
        void JsonParser::StartService()
        {
        }

        void JsonParser::StopService()
        {
        }

        void JsonParser::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("JsonParser", std::make_shared<JsonParser>());
            logger::Logger::LOG("JsonParser::Initialize", "Initialization successful!");
        }

        /// @brief Creating JSON object from the string
        /// @param jsonString 
        /// @return created JSON object
        nlohmann::json JsonParser::ParseJsonFromString(std::string const& jsonString){
            nlohmann::json parsedJson_;
            try
            {
                parsedJson_ = nlohmann::json::parse(jsonString);
            }
            catch(const std::exception& e)
            {
                logger::Logger::LOG("SensorDataReceiver::ProcessReceivedUdpData", e.what(), logger::LOGLEVEL::ERROR_LEVEL);
                return {};
            }
            return parsedJson_;
        }

        /// @brief Parsing the JSON and validating it, then fill the struct SensorData with parsed data
        /// @param parsedJson 
        /// @param data 
        /// @return true if validation and parsing is success
        bool JsonParser::ParseAndValidateUDPSensorData(std::string const& jsonString, sensordatareceiver::SensorData& data){
            nlohmann::json parsedJson = ParseJsonFromString(jsonString);
            if(parsedJson.is_null()){
                logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received empty JSON", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received sensor data [JSON.dump()]: " + parsedJson.dump(), logger::LOGLEVEL::DEBUG_LEVEL);
            if(!parsedJson.contains("sensor_id") || !parsedJson.at("sensor_id").is_string()){
                logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received JSON with invalid sensor_id details", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            if(!parsedJson.contains("timestamp") || !parsedJson.at("timestamp").is_number()){
                logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received JSON with invalid timestamp details", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            // if the motion_detected value is false, we can ignore the current received data. We will be handling only the motion detected data,
            // Only the motion detected true payload will have motion intensity and temparature details
            if(!parsedJson.contains("motion_detected") || !parsedJson.at("motion_detected").is_boolean()
                || !parsedJson.at("motion_detected")
            ){
                logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received JSON with invalid motion_detected details", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            if(!parsedJson.contains("motion_intensity") || !parsedJson.at("motion_intensity").is_number_float()){
                logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received JSON with invalid motion_intensity details", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            if(!parsedJson.contains("temperature") || !parsedJson.at("temperature").is_number_float()){
                logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received JSON with invalid temperature details", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            if(!parsedJson.contains("battery_level") || !parsedJson.at("battery_level").is_number()){
                logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received JSON with invalid battery_level details", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            if(!parsedJson.contains("zone_id") || !parsedJson.at("zone_id").is_string()){
                logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received JSON with invalid zone_id details", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received sensor data JSON validated successfully");
            data.sensorId_ = parsedJson["sensor_id"];
            data.timeStamp_ = parsedJson["timestamp"];
            data.motionIntensity_ = parsedJson["motion_intensity"];
            data.temperature_= parsedJson["temperature"];
            data.batteryLevel_ = parsedJson["battery_level"];
            data.zoneId_ = parsedJson["zone_id"];
            logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received Data: Sensor ID = " + data.sensorId_ + "; timestamp"
                " =" + std::to_string(data.timeStamp_) + "; Motion Intensity = " + std::to_string(data.motionIntensity_) + "; Temperature"
                " =" + std::to_string(data.temperature_) + "; Battery level = " + std::to_string(data.batteryLevel_) + ""
                "; Zone id = " + data.zoneId_, logger::LOGLEVEL::DEBUG_LEVEL
            );
            return true;
        }

        void JsonParser::Finalize()
        {
        }
    }
}
