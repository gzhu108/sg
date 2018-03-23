#pragma once
#ifndef streetgangworldapi_RequestGetWorld
#define streetgangworldapi_RequestGetWorld

#include "MessageBase.h"
#include "Rectangle.h"
#include "WorldId.h"


namespace streetgangworldapi
{
    class RequestGetWorld : public MessageBase
    {
    public:
        RequestGetWorld();
        virtual ~RequestGetWorld();
        
        PROPERTY(WorldId, streetgangworldapi::WorldId, streetgangworldapi::DefaultWorldId);

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangworldapi_RequestGetWorld
