#pragma once
#ifndef streetgangserver_RequestByebyeReactor
#define streetgangserver_RequestByebyeReactor

#include "MessageReactor.h"
#include "StreetGangResponseEncoder.h"
#include "RequestByebye.h"


namespace streetgangserver
{
    class RequestByebyeReactor : public sg::microreactor::MessageReactor<streetgangapi::RequestByebye, streetgangapi::StreetGangResponseEncoder>
    {
    public:
        RequestByebyeReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestByebye> message);
        virtual ~RequestByebyeReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // streetgangserver_RequestByebyeReactor
