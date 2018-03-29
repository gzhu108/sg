#pragma once
#ifndef worldapi_ResponseCreateWorld
#define worldapi_ResponseCreateWorld

#include "WorldMessage.h"
#include "WorldId.h"


namespace worldapi
{
    class ResponseCreateWorld : public WorldMessage
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
