#ifndef CONFIGPARSER_HEADER
#define CONFIGPARSER_HEADER

#include <Events/IEvents.hpp>
#include <Logger/Logger.hpp>
#include <nlohmann/json.hpp>
#include <SystemContext/SharedDataStore.hpp>
#include <SystemContext/ComponentRegistry.hpp>
#include <JsonParser/JsonParser.hpp>
#include <fstream>

namespace sensormoniteringhub{
    namespace jsonparser{
        /// @brief ConfigParser class responsible for loading and setting default configurations for the Sensor Monitoring Hub Manager.
        class ConfigParser: public IEvents{
            private:
            static std::string configPath_;
            public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
            /// @brief Loads configurations from a specified JSON file path.
            /// @param configPath The file path to the JSON configuration file.
            /// @return true if the configurations were loaded successfully, false otherwise.
            static bool LoadConfigs(std::string const& configPath);
            /// @brief Sets default configurations for the Sensor Monitoring Hub Manager in case loading from file fails.
            static void SetDefaultConfigs();
            /// @brief Method for changing the config file
            bool ChangeConfigFile(std::string const& configString);
        };
    }
}

#endif