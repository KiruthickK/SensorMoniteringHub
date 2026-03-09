#ifndef DATAPOOL_HEADER
#define DATAPOOL_HEADER

#include "IEvents.hpp"
namespace sensormoniteringhub{
    namespace datapool{
        class DataPool : public IEvents{
            public:
            virtual void StartService();
            virtual void StopService();
        };
    }
}

#endif