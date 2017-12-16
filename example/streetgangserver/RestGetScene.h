#pragma once
#ifndef streetgangserver_RestGetScene
#define streetgangserver_RestGetScene

#include "RestFactory.h"


namespace streetgangserver
{
    class RestGetScene : public sg::microreactor::RestFactory
    {
    public:
        RestGetScene();
        virtual ~RestGetScene();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;
    };
}


#endif // streetgangserver_RestGetScene
