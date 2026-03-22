#ifndef REQUEST_DATA_HEADER
#define REQUEST_DATA_HEADER
#include <iostream>
#include <stdint.h>
namespace sensormoniteringhub{
    namespace clientrequestservice{
        enum class RequestType : uint8_t{
            GET_EVENTS = 1,
            GET_LATEST = 2,
            GET_ZONES = 3,
            GET_SENSOR_STATUS = 4,
            GET_STATS = 5,
            UNKNOWN = 6
        };

        /// @brief returns the enum type of RequestType from string
        /// @param str 
        /// @return enum type of the given request
        inline RequestType GetRequestType(std::string const& str){
            if(str == "GET_EVENTS"){
                return RequestType::GET_EVENTS;
            }else if(str == "GET_LATEST"){
                return RequestType::GET_LATEST;
            }else if(str == "GET_ZONES"){
                return RequestType::GET_ZONES;
            }else if(str == "GET_SENSOR_STATUS"){
                return RequestType::GET_SENSOR_STATUS;
            }else if(str == "GET_STATS"){
                return RequestType::GET_STATS;
            }
            return RequestType::UNKNOWN;
        }
        
        struct RequestData{
            RequestType reqType_{RequestType::UNKNOWN};
            std::string reqId_{""};
            std::string zone_id_{""};
            uint64_t from_time_{0U};
            uint64_t to_time_{0U};
            uint64_t limit_{0U};
            std::string op_format_{""};
        };
    }
}
#endif