#ifndef REQUEST_PARSER_HEADER
#define REQUEST_PARSER_HEADER
#include "IEvents.hpp"
namespace sensormoniteringhub{
    namespace clientrequestservice{
        class RequestParser : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
        };
    }
}

#endif