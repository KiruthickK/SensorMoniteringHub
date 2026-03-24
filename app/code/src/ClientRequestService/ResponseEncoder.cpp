#include "ClientRequestService/ResponseEncoder.hpp"

namespace sensormoniteringhub{
    namespace clientrequestservice{
        void ResponseEncoder::StartService()
        {
        }
        void ResponseEncoder::StopService()
        {
        }
        void ResponseEncoder::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("ResponseEncoder", std::make_shared<ResponseEncoder>());
            logger::Logger::LOG("ResponseEncoder::Initialize", "Initialization successful!");
        }

        /// @brief routes the container to json and routes back the json dumped string
        /// @param responseData 
        /// @param reqData 
        /// @return encoded response string
        std::string ResponseEncoder::EncodeResponseToString(SensorInput const& responseData, RequestData const& reqData){
            std::string encodedString{""};
            auto jsonParserInstance{
                std::dynamic_pointer_cast<jsonparser::JsonParser>(
                    systemcontext::ComponentRegistry::GetComponent("JsonParser")
                )
            };
            if(!jsonParserInstance){
                logger::Logger::LOG("ResponseEncoder::EncodeResponseToString", "JsonParser object is not available", logger::LOGLEVEL::ERROR_LEVEL);
                return "";
            }
            std::visit([&](const auto& resData){
                if constexpr (std::is_same_v<std::decay_t<decltype(resData)>, std::vector<sensordatareceiver::SensorData>>){
                    if(resData.empty()){
                        logger::Logger::LOG("ResponseEncoder::EncodeResponseToString", "responseData Container is empty", logger::LOGLEVEL::ERROR_LEVEL);
                        return;
                    }
                    encodedString = jsonParserInstance->SerializeResponseToTCPClientForGetEvents(resData, reqData);
                } else if constexpr (std::is_same_v<std::decay_t<decltype(resData)>, sensordatareceiver::SensorData>){
                    if(resData.sensorId_.empty()){
                        logger::Logger::LOG("ResponseEncoder::EncodeResponseToString", "responseData is empty", logger::LOGLEVEL::ERROR_LEVEL);
                        return;
                    }
                    encodedString = jsonParserInstance->SerializeResponseToTCPClientForGetLatest(resData, reqData);
                } else if constexpr(std::is_same_v<std::decay_t<decltype(resData)>,std::pair<bool, std::string>>){
                    if(resData.second.empty()){
                        logger::Logger::LOG("ResponseEncoder::EncodeResponseToString", "responseData is empty", logger::LOGLEVEL::ERROR_LEVEL);
                        return;
                    }
                    encodedString = jsonParserInstance->SerializeResponseToTCPClientForGetSensorStatus(resData, reqData);
                } else if constexpr(std::is_same_v<std::decay_t<decltype(resData)>, std::pair<uint16_t, size_t>>){
                    encodedString = jsonParserInstance->SerializeResponseToTCPClientForGetStats(resData, reqData);
                }
            }, responseData);
            return encodedString;
        }

        void ResponseEncoder::Finalize()
        {
        }
    }
}

