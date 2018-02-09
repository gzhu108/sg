#pragma once
#ifndef streetgangserver_RequestGetVersionReactor
#define streetgangserver_RequestGetVersionReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "RequestGetVersion.h"


namespace streetgangserver
{
    class RequestGetVersionReactor : public sg::microreactor::MessageReactor<streetgangapi::RequestGetVersion>
    {
    public:
        RequestGetVersionReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestGetVersion> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestGetVersionReactor();

    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangResponder> mResponder;
    };
}


#endif // streetgangserver_RequestGetVersionReactor
