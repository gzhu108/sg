#pragma once
#ifndef simplewebserver_GetFileReactor
#define simplewebserver_GetFileReactor

#include "RestService.h"


namespace simplewebserver
{
    class GetFileReactor : public sg::microreactor::Reactor
    {
    public:
        GetFileReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~GetFileReactor();

    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<sg::microreactor::RestRequest> mRequest;
    };
}


#endif // simplewebserver_GetFileReactor
