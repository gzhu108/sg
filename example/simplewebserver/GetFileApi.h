#pragma once
#ifndef simplewebserver_GetFileApi
#define simplewebserver_GetFileApi

#include "RestService.h"


namespace simplewebserver
{
    class GetFileApi : public sg::microreactor::RestApi
    {
    public:
        GetFileApi();
        virtual ~GetFileApi();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;
    };
}


#endif // simplewebserver_GetFileApi
