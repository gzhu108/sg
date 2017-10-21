#pragma once
#ifndef streetgangserver_RequestErrorReactor
#define streetgangserver_RequestErrorReactor

#include "MessageReactor.h"
#include "StreetGangResponseEncoder.h"
#include "ResponseError.h"


namespace streetgangserver
{
    class RequestErrorReactor : public sg::microreactor::MessageReactor<streetgangapi::ResponseError, streetgangapi::StreetGangResponseEncoder>
    {
    public:
        RequestErrorReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseError> message);
        virtual ~RequestErrorReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // streetgangserver_RequestErrorReactor
