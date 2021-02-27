#pragma once
#ifndef streetgangserver_RequestByebyeReactor
#define streetgangserver_RequestByebyeReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "streetgangapi/RequestByebye.h"
#include "StreetGangReactor.h"


namespace streetgangserver
{
    class RequestByebyeReactor
        : public microreactor::MessageReactor<streetgangapi::RequestByebye>
        , public StreetGangReactor
    {
    public:
        RequestByebyeReactor(microreactor::Connection& connection, std::shared_ptr<streetgangapi::RequestByebye> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestByebyeReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // streetgangserver_RequestByebyeReactor
