#pragma once
#ifndef streetgangserver_RequestGetSceneReactor
#define streetgangserver_RequestGetSceneReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "RequestGetScene.h"
#include "WorldServiceProvider.h"


namespace streetgangserver
{
    class RequestGetSceneReactor : public sg::microreactor::MessageReactor<streetgangapi::RequestGetScene>
    {
    public:
        RequestGetSceneReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestGetScene> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestGetSceneReactor();
        
    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangResponder> mResponder;
    };
}


#endif // streetgangserver_RequestGetSceneReactor
