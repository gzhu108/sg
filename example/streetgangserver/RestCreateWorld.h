#pragma once
#ifndef streetgangserver_RestCreateWorld
#define streetgangserver_RestCreateWorld

#include "RestApi.h"


namespace streetgangserver
{
    class RestCreateWorld : public sg::microreactor::RestApi
    {
    public:
        RestCreateWorld();
        virtual ~RestCreateWorld();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;
    };
}


#endif // streetgangserver_RestCreateWorld
