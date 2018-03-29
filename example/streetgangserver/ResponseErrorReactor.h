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
        ResponseErrorReactor(sg::microreactor::Connection& connection, std::shared_ptr<worldapi::ResponseError> message, std::shared_ptr<StreetGangReactor> reactor);
        virtual ~ResponseErrorReactor();

    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<StreetGangReactor> mReactor;
    };
}


#endif // streetgangserver_ResponseErrorReactor
