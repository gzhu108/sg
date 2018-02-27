#pragma once
#ifndef myserver_MyService
#define myserver_MyService

#include "MyServerServiceBase.h"


namespace myserver
{
    class MyService : public MyServerServiceBase
    {
    public:
        explicit MyService(std::shared_ptr<sg::microreactor::Endpoint> endpoint);
        virtual ~MyService();

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGETv1versionReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;
        virtual std::shared_ptr<sg::microreactor::Reactor> CreatePOSTv1updateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;
    };
}


#endif // myserver_MyService
