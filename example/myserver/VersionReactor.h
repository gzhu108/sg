#pragma once
#ifndef myserver_VersionReactor
#define myserver_VersionReactor

#include "GETv1versionReactorBase.h"


namespace myserver
{
    class VersionReactor : public GETv1versionReactorBase
    {
    public:
        VersionReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~VersionReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // myserver_VersionReactor
