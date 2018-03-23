#pragma once
#ifndef streetgangworldapi_ResponseGetWorld
#define streetgangworldapi_ResponseGetWorld

#include <vector>
#include "MessageBase.h"
#include "Rectangle.h"
#include "Point.h"
#include "WorldId.h"


namespace streetgangworldapi
{
    class ResponseGetWorld : public MessageBase
    {
    public:
        ResponseGetWorld();
        virtual ~ResponseGetWorld();
        
        PROPERTY(WorldId, streetgangworldapi::WorldId, streetgangworldapi::DefaultWorldId);
        PROPERTY(Rect, streetgangworldapi::Rectangle<float>, {0.0, 0.0, 0.0, 0.0});
        PROPERTY(Items, std::vector<streetgangworldapi::Point<float>>);

    public:
        virtual bool Encode(std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
    };
}


#endif // streetgangworldapi_ResponseGetWorld
