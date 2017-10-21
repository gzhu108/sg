#pragma once
#ifndef bouncer_RestBouncer
#define bouncer_RestBouncer

#include "RestApi.h"


namespace bouncer
{
    class RestBouncer : public sg::microreactor::RestApi
    {
    public:
        RestBouncer();
        virtual ~RestBouncer();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;

    protected:
        virtual void RespondWithError(sg::microreactor::Connection& connection, uint32_t statusCode, const std::string& statusText);
    };
}


#endif // bouncer_RestBouncer
