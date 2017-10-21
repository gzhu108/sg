#pragma once
#ifndef streetgangclient_ResponseErrorReactor
#define streetgangclient_ResponseErrorReactor

#include "MessageReactor.h"
#include "StreetGangRequestEncoder.h"
#include "ResponseError.h"


namespace streetgangclient
{
    class ResponseErrorReactor : public sg::microreactor::MessageReactor<streetgangapi::ResponseError, streetgangapi::StreetGangRequestEncoder>
    {
    public:
        ResponseErrorReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseError> message);
        virtual ~ResponseErrorReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // streetgangclient_ResponseErrorReactor
