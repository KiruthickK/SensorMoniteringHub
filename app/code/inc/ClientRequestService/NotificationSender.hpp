#ifndef NOTIFICATION_SENDER_HEADER
#define NOTIFICATION_SENDER_HEADER
#include "IEvents.hpp"
namespace sensormoniteringhub{
    namespace clientrequestservice{
        class NotificationSender : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
        };
    }
}

#endif