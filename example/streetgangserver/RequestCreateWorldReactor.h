#pragma once
#ifndef streetgangserver_RequestCreateWorldReactor
#define streetgangserver_RequestCreateWorldReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "RequestCreateWorld.h"


namespace streetgangserver
{
    class RequestCreateWorldReactor : public sg::microreactor::MessageReactor<streetgangapi::RequestCreateWorld>
    {
    public:
        RequestCreateWorldReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestCreateWorld> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestCreateWorldReactor();
        
    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangResponder> mResponder;
    };
}


#endif // streetgangserver_RequestCreateWorldReactor
