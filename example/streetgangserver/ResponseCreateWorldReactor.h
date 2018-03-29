#pragma once
#ifndef streetgangserver_ResponseCreateWorldReactor
#define streetgangserver_ResponseCreateWorldReactor

#include "MessageReactor.h"
#include "worldapi/ResponseCreateWorld.h"
#include "RequestCreateWorldReactor.h"


namespace streetgangserver
{
    class ResponseCreateWorldReactor : public sg::microreactor::MessageReactor<worldapi::ResponseCreateWorld>
    {
    public:
        ResponseCreateWorldReactor(sg::microreactor::Connection& connection, std::shared_ptr<worldapi::ResponseCreateWorld> message, std::shared_ptr<RequestCreateWorldReactor> reactor);
        virtual ~ResponseCreateWorldReactor();
        
    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<RequestCreateWorldReactor> mReactor;
    };
}


#endif // streetgangserver_ResponseCreateWorldReactor
