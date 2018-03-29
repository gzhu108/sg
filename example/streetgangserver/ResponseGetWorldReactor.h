#pragma once
#ifndef streetgangserver_ResponseGetWorldReactor
#define streetgangserver_ResponseGetWorldReactor

#include "MessageReactor.h"
#include "worldapi/ResponseGetWorld.h"
#include "RequestGetSceneReactor.h"


namespace streetgangserver
{
    class ResponseGetWorldReactor : public sg::microreactor::MessageReactor<worldapi::ResponseGetWorld>
    {
    public:
        ResponseGetWorldReactor(sg::microreactor::Connection& connection, std::shared_ptr<worldapi::ResponseGetWorld> message, std::shared_ptr<RequestGetSceneReactor> reactor);
        virtual ~ResponseGetWorldReactor();
        
    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<RequestGetSceneReactor> mReactor;
    };
}


#endif // streetgangserver_ResponseGetWorldReactor
