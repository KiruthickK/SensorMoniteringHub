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
            GET_STATS = 5
        };
        struct RequestData{
            RequestType reqType_;
            std::string reqId_;
            std::string zone_id_;
            uint64_t from_time_;
            uint64_t to_time_;
            uint64_t limit_;
            std::string op_format_;
        };
    }
}
#endif