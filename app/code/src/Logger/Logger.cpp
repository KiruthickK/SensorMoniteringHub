#include "Logger/Logger.hpp"

namespace sensormoniteringhub{
    namespace logger{
        std::shared_ptr<Logger> Logger::LoggerInstance_ = nullptr;
        void Logger::StartService()
        {
            std::string filename = generateFileName();
            LogsFileEntryThread_ = std::thread([this, filename](){
                std::error_code ec;
                std::filesystem::create_directories("../logs", ec);
                if(ec){
                    Logger::LOG("Logger::StartService", "Failed to create directory!", logger::LOGLEVEL::ERROR_LEVEL);
                    return;
                }
                std::ofstream file("../logs/"+ filename, std::ios::app);
                if (!file)
                {
                    Logger::LOG("Logger::StartService", "Failed to open file!", logger::LOGLEVEL::ERROR_LEVEL);
                    return;
                }
                // thread will write all the logs before terminating the application
                while(!StopLoggingThread_.load() || !LogMessageQueue_.empty()){
                    std::string curMessage{""};
                    {
                        std::unique_lock<std::mutex> lock(LogMessageQueueMutex_);
                        LogQueuePushNotifierCV_.wait(lock, [this](){
                            return !LogMessageQueue_.empty() || StopLoggingThread_.load();
                        });
                        if(!LogMessageQueue_.empty()){
                            curMessage = LogMessageQueue_.front();
                            LogMessageQueue_.pop();
                        }
                    }
                    if(!curMessage.empty()){
                        file << curMessage << std::endl;
                    }
                }
                file << "Logger stopped. Application requested for stopping Logger service" << std::endl;
                file.close();
            });
        }

        void Logger::StopService()
        {
            StopLoggingThread_.store(true);
            LogQueuePushNotifierCV_.notify_one();
            if(LogsFileEntryThread_.joinable()){
                LogsFileEntryThread_.join();
            }
            std::cout<<"LOGGER Thread joined!" <<std::endl;
        }
        void Logger::Initialize()
        {
            LoggerInstance_ = std::make_shared<Logger>();
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(systemcontext::ComponentRegistry::GetComponent("ComponentRegistry"))->RegisterComponent("Logger", LoggerInstance_);
            LOG("Logger::Initialize", "Initialization successful!");
        }

        /// @brief 
        void Logger::Finalize()
        {
            LoggerInstance_ = nullptr;
        }

        /// @brief 
        /// @param metaData 
        /// @param str 
        /// @param level 
        void Logger::LOG(std::string metaData, std::string str, LOGLEVEL level, int errCode)
        {
            std::string message{"["+metaData+"] "+str};
            if(LoggerInstance_ == nullptr){
                std::cerr << RED << "[ERROR][Logger::LOG] Logger module is not initialized!" << RESET << std::endl;
                return;
            }
            switch (level)
            {
            case LOGLEVEL::INFO_LEVEL:
                message = "[INFO]" + message;
                std::cout << GREEN << message << RESET << std::endl;
                break;
            case LOGLEVEL::DEBUG_LEVEL:
                message = "[DEBUG]" + message;
                break;
            case LOGLEVEL::ERROR_LEVEL:
                message = "[ERROR]" + message;
                std::cerr << RED << message << RESET << std::endl;
                break;
            case LOGLEVEL::WARNING_LEVEL:
                message = "[WARNING!]" + message;
                std::cerr << YELLOW << message << RESET << std::endl;
            default:
                break;
            }
            {
                std::lock_guard<std::mutex> lock(LoggerInstance_->LogMessageQueueMutex_);
                LoggerInstance_->LogMessageQueue_.push(message);
                LoggerInstance_->LogQueuePushNotifierCV_.notify_one();
            }
        }

        /// @brief Generates a file name for the log file based on the current date and time.
        /// @return A string representing the generated log file name.
        std::string Logger::generateFileName()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            std::tm *timeInfo = std::localtime(&now_c);

            std::ostringstream filename;

            filename << "Log-"
                    << std::put_time(timeInfo, "%b-%d-%HHrs-%MMins")
                    << ".txt";

            return filename.str();
        }
    }
}
