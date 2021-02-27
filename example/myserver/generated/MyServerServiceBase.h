#pragma once
#ifndef myserver_MyServerServiceBase
#define myserver_MyServerServiceBase

#include "RestService.h"


namespace myserver
{
    class MyServerServiceBase : public microreactor::RestService
    {
    public:
        MyServerServiceBase(const std::string& hostAddress, uint16_t port);
        explicit MyServerServiceBase(std::shared_ptr<microreactor::Profile> profile);
        explicit MyServerServiceBase(std::shared_ptr<microreactor::Listener> listener);
        virtual ~MyServerServiceBase();

    public:
        virtual bool Initialize() override;

    protected:
        virtual std::shared_ptr<microreactor::Reactor> CreateGETv1versionReactor(std::shared_ptr<microreactor::RestMessage> request, microreactor::Connection& connection) = 0;
        virtual std::shared_ptr<microreactor::Reactor> CreatePOSTv1updateReactor(std::shared_ptr<microreactor::RestMessage> request, microreactor::Connection& connection) = 0;
    };
}


#endif // myserver_MyServerServiceBase
