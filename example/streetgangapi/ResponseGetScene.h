#pragma once
#ifndef streetgangapi_ResponseGetScene
#define streetgangapi_ResponseGetScene

#include <vector>
#include "MessageBase.h"
#include "Rectangle.h"
#include "Point.h"
#include "SessionId.h"


namespace streetgangapi
{
    class ResponseGetScene : public MessageBase
    {
    public:
        ResponseGetScene();
        virtual ~ResponseGetScene();
        
        PROPERTY(WorldId, streetgangapi::SessionId, streetgangapi::DefaultSessionId);
        PROPERTY(Rect, streetgangapi::Rectangle<float>, {0.0, 0.0, 0.0, 0.0});
        PROPERTY(Items, std::vector<streetgangapi::Point<float>>);
    };
}


#endif // streetgangapi_ResponseGetScene
