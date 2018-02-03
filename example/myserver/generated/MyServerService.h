#pragma once
#ifndef myserver_MyServerService
#define myserver_MyServerService

#include "Microreactor.h"


namespace myserver
{
    class MyServerService : public sg::microreactor::RestService
    {
    public:
        MyServerService(std::shared_ptr<sg::microreactor::Endpoint> endpoint, std::shared_ptr<sg::microreactor::Profile> profile);
        virtual ~MyServerService();

    public:
        virtual bool Initialize() override;

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGETv1versionReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreatePOSTv1updateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
    };
}


#endif // myserver_MyServerService
