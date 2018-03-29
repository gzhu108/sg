#pragma once
#ifndef streetgangserver_RequestGetSceneReactor
#define streetgangserver_RequestGetSceneReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "streetgangapi/RequestGetScene.h"
#include "WorldServiceProvider.h"
#include "StreetGangReactor.h"


namespace streetgangserver
{
    class RequestGetSceneReactor
        : public sg::microreactor::MessageReactor<streetgangapi::RequestGetScene>
        , public StreetGangReactor
    {
    public:
        RequestGetSceneReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestGetScene> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestGetSceneReactor();
        
    public:
        virtual bool Process() override;
        virtual bool SendResponse(const streetgangapi::SessionId& sessionId, const std::vector<streetgangapi::Point<float>>& items);
    };
}


#endif // streetgangserver_RequestGetSceneReactor
