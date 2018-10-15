#pragma once
#ifndef worldapi_ResponseGetWorld
#define worldapi_ResponseGetWorld

#include <vector>
#include "WorldMessage.h"
#include "World.h"


namespace worldapi
{
    class ResponseGetWorld : public WorldMessage
    {
    public:
        ResponseGetWorld();
        virtual ~ResponseGetWorld();
        
        PROPERTY(World, worldapi::World);

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // worldapi_ResponseGetWorld
