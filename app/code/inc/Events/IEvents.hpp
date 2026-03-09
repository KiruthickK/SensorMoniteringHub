#ifndef IEVENTS_HEADER
#define IEVENTS_HEADER

/// @brief namespace for the entire app codebase
namespace sensormoniteringhub{

    /// @brief Interface which all the classes under the app need to inherit
    class IEvents{
        public:
        /// @brief for starting the service during initialisation
        virtual void StartService() = 0;
        /// @brief for stopping the service during finalisation
        virtual void StopService() = 0;
    };
}

#endif