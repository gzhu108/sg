#pragma once
#ifndef worldapi_ResponseGetWorld
#define worldapi_ResponseGetWorld

#include <vector>
#include "MessageBase.h"
#include "Rectangle.h"
#include "Point.h"
#include "WorldId.h"


namespace worldapi
{
    class ResponseGetWorld : public MessageBase
    {
    public:
        ResponseGetWorld();
        virtual ~ResponseGetWorld();
        
        PROPERTY(WorldId, worldapi::WorldId, worldapi::DefaultWorldId);
        PROPERTY(Rect, worldapi::Rectangle<float>, {0.0, 0.0, 0.0, 0.0});
        PROPERTY(Items, std::vector<worldapi::Point<float>>);

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // worldapi_ResponseGetWorld
