#pragma once
#ifndef streetgangworldapi_RequestCreateWorld
#define streetgangworldapi_RequestCreateWorld

#include "MessageBase.h"


namespace streetgangworldapi
{
    class RequestCreateWorld : public MessageBase
    {
    public:
        RequestCreateWorld();
        virtual ~RequestCreateWorld();
        
        PROPERTY(WorldName, std::string, "New World");

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangworldapi_RequestCreateWorld
