#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include <iostream>
#include <memory>
#include <stdint.h>
#include <queue>
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>

namespace sensormoniteringhub{
    namespace logger{
        enum class LOGLEVEL : uint8_t {
            INFO_LEVEL = 1,
            DEBUG_LEVEL = 2,
            ERROR_LEVEL = 3
        };
        class Logger : public IEvents{
            private:
            static std::shared_ptr<Logger> LoggerInstance_;
            std::queue<std::string> LogMessageQueue_;       
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();    
            static void LOG(std::string metaData, std::string str, LOGLEVEL level = LOGLEVEL::INFO_LEVEL);
        };
    }
}

#endif