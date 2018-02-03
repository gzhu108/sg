#pragma once
#ifndef myserver_GETv1versionReactor
#define myserver_GETv1versionReactor

#include "Microreactor.h"
#include "GETv1versionMessage.h"


namespace myserver
{
    class GETv1versionReactor : public sg::microreactor::RestReactor<GETv1versionMessage>
    {
    public:
        GETv1versionReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~GETv1versionReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // myserver_GETv1versionReactor
