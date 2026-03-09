#include "Logger/Logger.hpp"

namespace sensormoniteringhub{
    namespace logger{
        std::shared_ptr<Logger> Logger::LoggerInstance_ = nullptr;
        void Logger::StartService()
        {
        }

        void Logger::StopService()
        {
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
        }

        /// @brief 
        /// @param metaData 
        /// @param str 
        /// @param level 
        void Logger::LOG(std::string metaData, std::string str, LOGLEVEL level)
        {
            std::string message{"["+metaData+"] "+str};
            if(LoggerInstance_ == nullptr){
                std::cerr << "\033[31m" << "[ERROR][Logger::LOG] Logger module is not initialized!" << "\033[0m" << std::endl;
                return;
            }
            switch (level)
            {
            case LOGLEVEL::INFO_LEVEL:
                message = "[INFO]" + message;
                std::cout << message << std::endl;
                LoggerInstance_->LogMessageQueue_.push(message);
                break;
            case LOGLEVEL::DEBUG_LEVEL:
                message = "[DEBUG]" + message;
                LoggerInstance_->LogMessageQueue_.push(message);
                break;
            case LOGLEVEL::ERROR_LEVEL:
                message = "[ERROR]" + message;
                std::cerr << "\033[31m" << message << "\033[0m" << std::endl;
                LoggerInstance_->LogMessageQueue_.push(message);
            default:
                break;
            }
        }
    }
}
