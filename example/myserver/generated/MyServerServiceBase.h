#pragma once
#ifndef myserver_MyServerServiceBase
#define myserver_MyServerServiceBase

#include "RestService.h"


namespace myserver
{
    class MyServerServiceBase : public sg::microreactor::RestService
    {
    public:
        MyServerServiceBase(std::shared_ptr<sg::microreactor::Endpoint> endpoint);
        virtual ~MyServerServiceBase();

    public:
        virtual bool Initialize() override;

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGETv1versionReactor(std::shared_ptr<sg::microreactor::RestRequest> request, std::shared_ptr<sg::microreactor::Connection> connection) = 0;
        virtual std::shared_ptr<sg::microreactor::Reactor> CreatePOSTv1updateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, std::shared_ptr<sg::microreactor::Connection> connection) = 0;
    };
}


#endif // myserver_MyServerServiceBase
