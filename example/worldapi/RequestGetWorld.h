#pragma once
#ifndef worldapi_RequestGetWorld
#define worldapi_RequestGetWorld

#include "MessageBase.h"
#include "Rectangle.h"
#include "WorldId.h"


namespace worldapi
{
    class RequestGetWorld : public MessageBase
    {
    public:
        RequestGetWorld();
        virtual ~RequestGetWorld();
        
        PROPERTY(WorldId, worldapi::WorldId, worldapi::DefaultWorldId);

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // worldapi_RequestGetWorld
