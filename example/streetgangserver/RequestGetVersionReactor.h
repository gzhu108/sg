#pragma once
#ifndef streetgangserver_RequestGetVersionReactor
#define streetgangserver_RequestGetVersionReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "streetgangapi/RequestGetVersion.h"
#include "StreetGangReactor.h"


namespace streetgangserver
{
    class RequestGetVersionReactor
        : public microreactor::MessageReactor<streetgangapi::RequestGetVersion>
        , public StreetGangReactor
    {
    public:
        RequestGetVersionReactor(microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestGetVersion> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestGetVersionReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // streetgangserver_RequestGetVersionReactor
