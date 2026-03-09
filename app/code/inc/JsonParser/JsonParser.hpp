#ifndef JSONPARSER_HEADER
#define JSONPARSER_HEADER

#include "IEvents.hpp"
namespace sensormoniteringhub{
    namespace jsonparser{
        class JsonParser : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
        };
    }
}

#endif