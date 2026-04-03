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
                return nullptr;
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
            logger::Logger::LOG("JsonParser::ParseAndValidateUDPSensorData", "Received sensor data [JSON.dump()]: \n" + parsedJson.dump(4), logger::LOGLEVEL::DEBUG_LEVEL);
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

        /// @brief parses the request received from tcp client and validates for mandatory param (request_type) and fills the RequestData struct
        /// @param jsonString 
        /// @param data 
        /// @return true if parsing success
        bool JsonParser::ParseRequestFromTCPClient(std::string const& jsonString, clientrequestservice::RequestData& data){
            nlohmann::json parsedJson = ParseJsonFromString(jsonString);
            if(parsedJson.is_null()){
                logger::Logger::LOG("JsonParser::ParseRequestFromTCPClient", "Received empty JSON", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            logger::Logger::LOG("JsonParser::ParseRequestFromTCPClient", "Received request data [JSON.dump()]: \n" + parsedJson.dump(4), logger::LOGLEVEL::DEBUG_LEVEL);
            if(!parsedJson.contains("request_type") || !parsedJson.at("request_type").is_string()){
                logger::Logger::LOG("JsonParser::ParseRequestFromTCPClient", "Request_type is not present in the request, request json validation failed", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            if(!parsedJson.contains("request_id") || !parsedJson.at("request_id").is_string()){
                logger::Logger::LOG("JsonParser::ParseRequestFromTCPClient", "request_id is not present in the request, request json validation failed", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }
            data.reqType_ = clientrequestservice::GetRequestType(parsedJson.at("request_type").get<std::string>());
            data.reqId_= parsedJson.at("request_id");
            if(parsedJson.contains("zone_id") && parsedJson.at("zone_id").is_string()){
                data.zone_id_ = parsedJson.at("zone_id");
            }
            if(parsedJson.contains("from_time") && parsedJson.at("from_time").is_number()){
                data.from_time_ = parsedJson.at("from_time");
            }
            if(parsedJson.contains("to_time") && parsedJson.at("to_time").is_number()){
                data.to_time_= parsedJson.at("to_time");
            }
            if(parsedJson.contains("format") && parsedJson.at("format").is_string()){
                data.op_format_ = parsedJson.at("format");
            }
            if(parsedJson.contains("limit") && parsedJson.at("limit").is_number()){
                data.limit_= parsedJson.at("limit");
            }
            return true;
        }

        bool JsonParser::ParseCommandFromTCPClient(std::string const& jsonString, controlcommandservice::CommandOrderInformation& data){
            nlohmann::json parsedJson = ParseJsonFromString(jsonString);
            if(parsedJson.is_null()){
                logger::Logger::LOG("JsonParser::ParseCommandFromTCPClient", "Received empty JSON", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            if(parsedJson.contains("order_id") && parsedJson.at("order_id").is_string()){
                data.orderId_ = parsedJson.at("order_id");
            }else{
                logger::Logger::LOG("JsonParser::ParseCommandFromTCPClient", "OrderId is not present in the JSON", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            if(parsedJson.contains("order_type") && parsedJson.at("order_type").is_string()){
                data.type_ = controlcommandservice::GetCommandType(parsedJson.at("order_type"));
            }else{
                logger::Logger::LOG("JsonParser::ParseCommandFromTCPClient", "OrderType is not present in the JSON", logger::LOGLEVEL::WARNING_LEVEL);
                return false;
            }
            return true;
        }

        /// @brief method to form json from SensorData objects and return the dumped string
        /// @param sensorDataContainer 
        /// @param reqData 
        /// @return 
        std::string JsonParser::SerializeResponseToTCPClientForGetEvents(std::vector <sensordatareceiver::SensorData> const& sensorDataContainer, clientrequestservice::RequestData const& reqData){
            nlohmann::json responseJson;
            if(sensorDataContainer.empty()){
                logger::Logger::LOG("JsonParser::SerializeResponseToTCPClientForGetEvents", "responseData Container is empty", logger::LOGLEVEL::ERROR_LEVEL);
                return "";
            }
            responseJson["response_id"] = reqData.reqId_;
            if(!reqData.zone_id_.empty()){
                responseJson["zone_id"] = reqData.zone_id_;
            }
            if(reqData.from_time_ > 0 && reqData.to_time_ > 0){
                responseJson["from_time"] = reqData.from_time_;
                responseJson["to_time"] = reqData.to_time_;
            }
            responseJson["sensor_data_readings"] = nlohmann::json::array();
            for(auto const& sensorData : sensorDataContainer){
                nlohmann::json sensor_data_reading;
                responseJson["status"] = "success";
                sensor_data_reading["timestamp"] = sensorData.timeStamp_;
                sensor_data_reading["motion_intensity"] = sensorData.motionIntensity_;
                sensor_data_reading["temperature"] = sensorData.temperature_;
                sensor_data_reading["battery_level"] = sensorData.batteryLevel_;
                sensor_data_reading["sensor_id"] = sensorData.sensorId_;
                responseJson["sensor_data_readings"].push_back(sensor_data_reading);
            }
            return responseJson.dump();
        }

        /// @brief method to form json from latest sensor data
        /// @param sensorDataContainer 
        /// @param reqData 
        /// @return dumped string of formed json object
        std::string JsonParser::SerializeResponseToTCPClientForGetLatest(sensordatareceiver::SensorData const& sensorDataContainer, clientrequestservice::RequestData const& reqData){
            nlohmann::json responseJson;
            responseJson["status"] = "success";
            responseJson["response_id"] = reqData.reqId_;
            responseJson["zone_id"] = sensorDataContainer.zoneId_;
            responseJson["timestamp"] = sensorDataContainer.timeStamp_;
            responseJson["motion_intensity"] = sensorDataContainer.motionIntensity_;
            responseJson["temperature"] = sensorDataContainer.temperature_;
            responseJson["battery_level"] = sensorDataContainer.batteryLevel_;
            responseJson["sensor_id"] = sensorDataContainer.sensorId_;
            return responseJson.dump();
        }

        /// @brief method to form json from sensor status
        /// @param pair 
        /// @param reqData 
        /// @return dumped string from the json
        std::string JsonParser::SerializeResponseToTCPClientForGetSensorStatus(std::pair<bool, std::string> pair, clientrequestservice::RequestData const& reqData){
            nlohmann::json responseJson;
            responseJson["status"] = "success";
            responseJson["response_id"] = reqData.reqId_;
            responseJson["sensor_status"] = (pair.first ? "active" : "inactive");
            responseJson["ecu_name"] = pair.second;
            return responseJson.dump();
        }

        /// @brief method to form json from the current stats
        /// @param pair 
        /// @param reqData 
        /// @return dumped string from the json
        std::string JsonParser::SerializeResponseToTCPClientForGetStats(std::pair<uint16_t, size_t> pair, clientrequestservice::RequestData const& reqData){
            nlohmann::json responseJson;
            responseJson["status"] = "success";
            responseJson["response_id"] = reqData.reqId_;
            responseJson["current_sensor_data_received"] = pair.first;
            responseJson["current_memory_used_int_bytes"] = pair.second;
            return responseJson.dump();
        }

        /// @brief method for forming JSON from set of zones we received from udp sensor data
        /// @param zones 
        /// @param reqData 
        /// @return dumped string from the json
        std::string JsonParser::SerializeResponseToTCPClientForGetZones(std::set<std::string> zones, clientrequestservice::RequestData const& reqData){
            nlohmann::json responseJson;
            responseJson["status"] = "success";
            responseJson["response_id"] = reqData.reqId_;
            responseJson["zones"] = zones; // set will automatically converted to JSON array
            return responseJson.dump();
        }

        /// @brief method for forming JSON for failure response for tcp control client
        /// @param failureReason 
        /// @return dumped string from the json
        std::string JsonParser::SerializeFailureResponseToTcpClient(std::string const& commandDataStr, std::string const& failureReason){
            nlohmann::json responseJson = ParseJsonFromString(commandDataStr); //with the command request json, we are just updating the status
            responseJson["status"] = "failed";
            responseJson["failure_reason"] = failureReason;
            return responseJson.dump();
        }

        /// @brief method for forming JSON for success response for tcp control client
        /// @param dataStr 
        /// @return dumped string from the json
        std::string JsonParser::SerializeSuccessResponseToTcpControlClient(std::string const& dataStr){
            nlohmann::json responseJson = ParseJsonFromString(dataStr); //with the command request json, we are just updating the status
            responseJson["status"] = "success";
            return responseJson.dump();
        }
        void JsonParser::Finalize()
        {
        }
    }
}
