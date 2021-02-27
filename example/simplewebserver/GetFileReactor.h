#pragma once
#ifndef simplewebserver_GetFileReactor
#define simplewebserver_GetFileReactor

#include "Microreactor.h"


namespace simplewebserver
{
    class GetFileReactor : public microreactor::RestReactor<microreactor::RestRequest>
    {
    public:
        GetFileReactor(microreactor::Connection& connection, std::shared_ptr<microreactor::RestRequest> request);
        virtual ~GetFileReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // simplewebserver_GetFileReactor
