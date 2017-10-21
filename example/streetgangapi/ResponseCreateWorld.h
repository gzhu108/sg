#pragma once
#ifndef streetgangapi_ResponseCreateWorld
#define streetgangapi_ResponseCreateWorld

#include "MessageBase.h"
#include "SessionId.h"


namespace streetgangapi
{
    class ResponseCreateWorld : public MessageBase
    {
    public:
        ResponseCreateWorld();
        virtual ~ResponseCreateWorld();
        
        PROPERTY(WorldId, streetgangapi::SessionId, streetgangapi::InvalidSessionId);
        PROPERTY(WorldName, std::string);
    };
}


#endif // streetgangapi_ResponseCreateWorld
