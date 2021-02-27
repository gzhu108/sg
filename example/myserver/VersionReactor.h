#pragma once
#ifndef myserver_VersionReactor
#define myserver_VersionReactor

#include "RestRequest.h"
#include "generated/GETv1versionReactorBase.h"


namespace myserver
{
    class VersionReactor : public GETv1versionReactorBase
    {
    public:
        VersionReactor(microreactor::Connection& connection, std::shared_ptr<microreactor::RestRequest> request);
        virtual ~VersionReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // myserver_VersionReactor
