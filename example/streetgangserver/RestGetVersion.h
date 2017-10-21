#pragma once
#ifndef streetgangserver_RestGetVersion
#define streetgangserver_RestGetVersion

#include "RestApi.h"


namespace streetgangserver
{
    class RestGetVersion : public sg::microreactor::RestApi
    {
    public:
        RestGetVersion();
        virtual ~RestGetVersion();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;
    };
}


#endif // streetgangserver_RestGetVersion
