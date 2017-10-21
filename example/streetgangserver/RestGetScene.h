#pragma once
#ifndef streetgangserver_RestGetScene
#define streetgangserver_RestGetScene

#include "RestApi.h"


namespace streetgangserver
{
    class RestGetScene : public sg::microreactor::RestApi
    {
    public:
        RestGetScene();
        virtual ~RestGetScene();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;
    };
}


#endif // streetgangserver_RestGetScene
