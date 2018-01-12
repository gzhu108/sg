#pragma once
#ifndef simplewebserver_SimpleWebService
#define simplewebserver_SimpleWebService

#include "RestService.h"


namespace simplewebserver
{
    class SimpleWebService : public sg::microreactor::RestService
    {
    public:
        SimpleWebService(std::shared_ptr<sg::microreactor::Endpoint> endpoint, std::shared_ptr<sg::microreactor::Profile> profile);
        virtual ~SimpleWebService();

    public:
        virtual bool Initialize() override;

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGetFileReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
    };
}


#endif // simplewebserver_SimpleWebService
