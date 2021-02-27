#pragma once
#ifndef myserver_MyService
#define myserver_MyService

#include "generated/MyServerServiceBase.h"


namespace myserver
{
    class MyService : public MyServerServiceBase
    {
    public:
        explicit MyService(std::shared_ptr<microreactor::Profile> profile);
        virtual ~MyService();

    protected:
        virtual std::shared_ptr<microreactor::Reactor> CreateGETv1versionReactor(std::shared_ptr<microreactor::RestMessage> request, microreactor::Connection& connection) override;
        virtual std::shared_ptr<microreactor::Reactor> CreatePOSTv1updateReactor(std::shared_ptr<microreactor::RestMessage> request, microreactor::Connection& connection) override;
    };
}


#endif // myserver_MyService
