#pragma once
#ifndef myserver_GETv1versionReactorBase
#define myserver_GETv1versionReactorBase

#include "RestReactor.h"


namespace myserver
{
    class GETv1versionReactorBase : public sg::microreactor::RestReactor<sg::microreactor::RestRequest>
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
