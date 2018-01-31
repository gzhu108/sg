#pragma once
#ifndef simplewebserver_GetFileReactor
#define simplewebserver_GetFileReactor

#include "Microreactor.h"


namespace simplewebserver
{
    class GetFileReactor : public sg::microreactor::RestReactor<sg::microreactor::RestMessage>
    {
    public:
        GetFileReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~GetFileReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // simplewebserver_GetFileReactor
