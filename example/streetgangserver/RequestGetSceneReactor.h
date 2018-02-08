#pragma once
#ifndef streetgangserver_RequestGetSceneReactor
#define streetgangserver_RequestGetSceneReactor

#include "MessageReactor.h"
#include "StreetGangResponseEncoder.h"
#include "StreetGangResponder.h"
#include "RequestGetScene.h"
#include "WorldServiceProvider.h"


namespace streetgangserver
{
    class RequestGetSceneReactor : public sg::microreactor::MessageReactor<streetgangapi::RequestGetScene, streetgangapi::StreetGangResponseEncoder>
    {
    public:
        RequestGetSceneReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestGetScene> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestGetSceneReactor();
        
    public:
        virtual bool Process() override;
        virtual void SetSession(std::shared_ptr<WorldServiceProvider> session) { mSession = session; }

    protected:
        std::shared_ptr<streetgangapi::StreetGangResponder> mResponder;
        std::shared_ptr<WorldServiceProvider> mSession;
    };
}


#endif // streetgangserver_RequestGetSceneReactor
