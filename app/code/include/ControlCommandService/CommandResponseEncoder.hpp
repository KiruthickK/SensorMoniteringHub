#ifndef COMMAND_RESPONSE_ENCODER
#define COMMAND_RESPONSE_ENCODER

#include <Events/IEvents.hpp>
#include <Logger/Logger.hpp>
#include <JsonParser/JsonParser.hpp>
#include <ControlCommandService/CommandOrderInformation.hpp>

namespace sensormoniteringhub{
    namespace controlcommandservice{
        class CommandResponseEncoder: public IEvents {
            public:
            static void Initialize();
            static void Finalize();
            virtual void StartService();
            virtual void StopService();
            std::string GetFailureResponse(std::string const& commandRequestStr, std::string const& failureReasonStr);
            std::string GetSuccessResponse(std::string const& commandRequestStr);
        };
    }
}

#endif