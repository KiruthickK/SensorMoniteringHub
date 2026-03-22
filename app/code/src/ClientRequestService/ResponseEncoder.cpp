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
        /// @return 
        std::string ResponseEncoder::EncodeResponseToString(std::vector<sensordatareceiver::SensorData> const& responseData, RequestData const& reqData){
            std::string encodedString{""};
            if(responseData.empty()){
                logger::Logger::LOG("ResponseEncoder::EncodeResponseToString", "responseData Container is empty", logger::LOGLEVEL::ERROR_LEVEL);
                return "";
            }
            auto jsonParserInstance{
                std::dynamic_pointer_cast<jsonparser::JsonParser>(
                    systemcontext::ComponentRegistry::GetComponent("JsonParser")
                )
            };
            if(!jsonParserInstance){
                logger::Logger::LOG("ResponseEncoder::EncodeResponseToString", "JsonParser object is not available", logger::LOGLEVEL::ERROR_LEVEL);
                return "";
            }
            return jsonParserInstance->SerializeResponseToTCPClientForGetEvents(responseData, reqData);
        }

        void ResponseEncoder::Finalize()
        {
        }
    }
}

