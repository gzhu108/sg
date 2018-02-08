#pragma once
#ifndef streetgangclient_ResponseErrorReactor
#define streetgangclient_ResponseErrorReactor

#include "MessageReactor.h"
#include "StreetGangRequestEncoder.h"
#include "StreetGangRequester.h"
#include "ResponseError.h"


namespace streetgangclient
{
    class ResponseErrorReactor : public sg::microreactor::MessageReactor<streetgangapi::ResponseError, streetgangapi::StreetGangRequestEncoder>
    {
    public:
        ResponseErrorReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseError> message, std::shared_ptr<streetgangapi::StreetGangRequester> requester);
        virtual ~ResponseErrorReactor();

    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequester> mRequester;
    };
}


#endif // streetgangclient_ResponseErrorReactor
