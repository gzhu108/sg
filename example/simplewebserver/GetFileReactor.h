#pragma once
#ifndef simplewebserver_GetFileReactor
#define simplewebserver_GetFileReactor

#include "RestService.h"


namespace simplewebserver
{
    class GetFileReactor : public sg::microreactor::RestReactor<sg::microreactor::Message>
    {
    public:
        GetFileReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request, std::shared_ptr<sg::microreactor::Message> input);
        virtual ~GetFileReactor();

    public:
        virtual bool Process() override;

    protected:
        std::shared_ptr<sg::microreactor::RestRequest> mRequest;
    };
}


#endif // simplewebserver_GetFileReactor
