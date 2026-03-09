#ifndef EVENTDISPATCHER_HEADER
#define EVENTDISPATCHER_HEADER

#include "IEvents.hpp"
namespace sensormoniteringhub{
    namespace eventdispatcher{
        class EventDispatcher : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
        };
    }
}

#endif