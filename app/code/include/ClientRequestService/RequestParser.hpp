#ifndef REQUEST_PARSER_HEADER
#define REQUEST_PARSER_HEADER
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <Logger/Logger.hpp>

namespace sensormoniteringhub{
    namespace clientrequestservice{
        class RequestParser : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
        };
    }
}

#endif