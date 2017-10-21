#pragma once
#ifndef streetgangapi_RequestGetVersion
#define streetgangapi_RequestGetVersion

#include "MessageBase.h"


namespace streetgangapi
{
    class RequestGetVersion : public MessageBase
    {
    public:
        RequestGetVersion();
        virtual ~RequestGetVersion();
    };
}


#endif // streetgangapi_RequestGetVersion
