#pragma once
#ifndef streetgangworldapi_ResponseCreateWorld
#define streetgangworldapi_ResponseCreateWorld

#include "MessageBase.h"
#include "WorldId.h"


namespace streetgangworldapi
{
    class ResponseCreateWorld : public MessageBase
    {
    public:
        ResponseCreateWorld();
        virtual ~ResponseCreateWorld();
        
        PROPERTY(WorldId, streetgangworldapi::WorldId, streetgangworldapi::InvalidWorldId);
        PROPERTY(WorldName, std::string);

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangworldapi_ResponseCreateWorld
