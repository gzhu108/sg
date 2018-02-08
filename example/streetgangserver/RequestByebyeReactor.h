#pragma once
#ifndef streetgangserver_RequestByebyeReactor
#define streetgangserver_RequestByebyeReactor

#include "MessageReactor.h"
#include "StreetGangResponseEncoder.h"
#include "StreetGangResponder.h"
#include "RequestByebye.h"


namespace streetgangserver
{
    class RequestByebyeReactor : public sg::microreactor::MessageReactor<streetgangapi::RequestByebye, streetgangapi::StreetGangResponseEncoder>
    {
    public:
        RequestByebyeReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestByebye> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestByebyeReactor();

    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangResponder> mResponder;
    };
}


#endif // streetgangserver_RequestByebyeReactor
