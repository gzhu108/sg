#pragma once
#ifndef streetgangserver_RequestGetVersionReactor
#define streetgangserver_RequestGetVersionReactor

#include "MessageReactor.h"
#include "StreetGangResponseEncoder.h"
#include "RequestGetVersion.h"


namespace streetgangserver
{
    class RequestGetVersionReactor : public sg::microreactor::MessageReactor<streetgangapi::RequestGetVersion, streetgangapi::StreetGangResponseEncoder>
    {
    public:
        RequestGetVersionReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestGetVersion> message);
        virtual ~RequestGetVersionReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // streetgangserver_RequestGetVersionReactor
