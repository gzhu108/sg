#pragma once
#ifndef worldapi_ResponseCreateWorld
#define worldapi_ResponseCreateWorld

#include "MessageBase.h"
#include "WorldId.h"


namespace worldapi
{
    class ResponseCreateWorld : public MessageBase
    {
    public:
        ResponseCreateWorld();
        virtual ~ResponseCreateWorld();
        
        PROPERTY(WorldId, worldapi::WorldId, worldapi::InvalidWorldId);
        PROPERTY(WorldName, std::string);

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // worldapi_ResponseCreateWorld
