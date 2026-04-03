#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include <iostream>
#include <memory>
#include <stdint.h>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <condition_variable>
#include <Events/IEvents.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <SystemContext/SharedDataStore.hpp>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define CYAN "\033[36m"

namespace sensormoniteringhub{
    namespace logger{
        /// @brief Defines the log levels for the Logger class.
        enum class LOGLEVEL : uint8_t {
            INFO_LEVEL = 1,
            DEBUG_LEVEL = 2,
            ERROR_LEVEL = 3,
            WARNING_LEVEL = 4,
            LIFECYCLE_LEVEL = 5
        };

        /// @brief Logger class responsible for logging messages with different log levels.
        class Logger : public IEvents{
            private:
            /// @brief A static shared pointer to the Logger instance.
            static std::shared_ptr<Logger> LoggerInstance_;
            /// @brief A queue to store log messages before they are processed.
            std::queue<std::string> LogMessageQueue_;
            /// @brief A thread to handle the processing of log messages from the queue.
            std::thread LogsFileEntryThread_;
            /// @brief An atomic boolean to signal the logging thread to stop when the Logger service is stopped.
            std::atomic<bool> StopLoggingThread_{false};
            /// @brief for preventing race conditions while using the queue
            std::mutex LogMessageQueueMutex_;
            /// @brief for notifying when the queue is being pushed
            std::condition_variable LogQueuePushNotifierCV_;
            /***
             * @brief variable to store the generated log file name, so in case of 
             * restarting, we have to use the first generated log file name, to prevent multiple files being created
             * for single run of application
             */
            std::string GeneratedLogFileName{""};
            
            public:
            /// @brief Initializes the Logger instance.
            virtual void StartService();
            /// @brief Stops the Logger service.
            virtual void StopService();
            /// @brief Initializes the Logger instance.
            static void Initialize();
            /// @brief Finalizes the Logger instance.
            static void Finalize();    
            /// @brief Logs a message with the specified metadata and log level.
            /// @param metaData Metadata associated with the log message.
            /// @param str The log message to be logged.
            /// @param level The log level for the message (default is INFO_LEVEL).
            /// @param errCode for processing the error if occurs
            static void LOG(std::string metaData, std::string str, LOGLEVEL level = LOGLEVEL::INFO_LEVEL, int errCode = 0);
            /// @brief Generates a file name for the log file based on the current date and time.
            /// @return A string representing the generated log file name.
            std::string generateFileName();
        };
    }
}

#endif