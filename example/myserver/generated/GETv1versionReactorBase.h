#pragma once
#ifndef myserver_GETv1versionReactorBase
#define myserver_GETv1versionReactorBase

#include "RestReactor.h"
#include "GETv1versionRequestContent.h"


namespace myserver
{
    class GETv1versionReactorBase : public microreactor::RestReactor<microreactor::RestRequest>
    {
    public:
        GETv1versionReactorBase(microreactor::Connection& connection, std::shared_ptr<microreactor::RestRequest> request);
        virtual ~GETv1versionReactorBase();

    public:
        // Override Process() in the derived class
        virtual bool Process() override;

    protected:
        GETv1versionRequestContent mRequestContent;
    };
}


#endif // myserver_GETv1versionReactorBase
