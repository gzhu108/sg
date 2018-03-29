#pragma once
#ifndef streetgangserver_ResponseErrorReactor
#define streetgangserver_ResponseErrorReactor

#include "MessageReactor.h"
#include "worldapi/ResponseError.h"
#include "StreetGangReactor.h"


namespace streetgangserver
{
    class ResponseErrorReactor : public sg::microreactor::MessageReactor<worldapi::ResponseError>
    {
    public:
        ResponseErrorReactor(sg::microreactor::Connection& connection, std::shared_ptr<worldapi::ResponseError> message);
        virtual ~ResponseErrorReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // streetgangserver_ResponseErrorReactor
