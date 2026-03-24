#ifndef ICLIENTREQUESTSERVICE_HEADER
#define ICLIENTREQUESTSERVICE_HEADER
#include <iostream>
class IClientRequestHandler
{
public:
    virtual std::string HandleRequest(const std::string& request) = 0;
    virtual ~IClientRequestHandler() = default;
};
#endif