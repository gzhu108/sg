#pragma once
#ifndef streetgangapi_ResponseGetVersion
#define streetgangapi_ResponseGetVersion

#include "MessageBase.h"


namespace streetgangapi
{
    class ResponseGetVersion : public MessageBase
    {
    public:
        ResponseGetVersion();
        virtual ~ResponseGetVersion();
        
        PROPERTY(Version, std::string, "0.0.0.0");
    };
}


#endif // streetgangapi_ResponseGetVersion
