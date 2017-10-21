#pragma once
#ifndef streetgangapi_RequestCreateWorld
#define streetgangapi_RequestCreateWorld

#include "MessageBase.h"


namespace streetgangapi
{
    class RequestCreateWorld : public MessageBase
    {
    public:
        RequestCreateWorld();
        virtual ~RequestCreateWorld();
        
        PROPERTY(WorldName, std::string, "New World");
    };
}


#endif // streetgangapi_RequestCreateWorld
