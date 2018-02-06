#pragma once
#ifndef myserver_GETv1versionReactorBase
#define myserver_GETv1versionReactorBase

#include "RestReactor.h"
#include "GETv1versionMessage.h"


namespace myserver
{
    class GETv1versionReactorBase : public sg::microreactor::RestReactor<GETv1versionMessage>
    {
    public:
        GETv1versionReactorBase(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~GETv1versionReactorBase();

    public:
        // Override Process() in the derived class
        virtual bool Process() override;
    };
}


#endif // myserver_GETv1versionReactorBase
