#pragma once
#ifndef streetgangapi_RequestGetScene
#define streetgangapi_RequestGetScene

#include "MessageBase.h"
#include "Rectangle.h"
#include "SessionId.h"


namespace streetgangapi
{
    class RequestGetScene : public MessageBase
    {
    public:
        RequestGetScene();
        virtual ~RequestGetScene();
        
        PROPERTY(WorldId, streetgangapi::SessionId, streetgangapi::DefaultSessionId);
        PROPERTY(Rect, streetgangapi::Rectangle<float>, {0.0, 0.0, 0.0, 0.0});
    };
}


#endif // streetgangapi_RequestGetScene
