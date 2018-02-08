#pragma once
#ifndef streetgangserver_RequestErrorReactor
#define streetgangserver_RequestErrorReactor

#include "MessageReactor.h"
#include "StreetGangResponseEncoder.h"
#include "StreetGangResponder.h"
#include "ResponseError.h"


namespace streetgangserver
{
    class RequestErrorReactor : public sg::microreactor::MessageReactor<streetgangapi::ResponseError, streetgangapi::StreetGangResponseEncoder>
    {
    public:
        RequestErrorReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseError> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestErrorReactor();

    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangResponder> mResponder;
    };
}


#endif // streetgangserver_RequestErrorReactor
