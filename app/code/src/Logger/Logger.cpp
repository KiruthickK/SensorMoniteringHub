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
            LoggerInstance_->LogMessageQueue_.push(message);
        }
    }
}
