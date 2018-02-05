#pragma once
#ifndef myserver_VersionReactor
#define myserver_VersionReactor

#include "Microreactor.h"
#include "GETv1versionReactor.h"


namespace myserver
{
    class VersionReactor : public GETv1versionReactor
    {
    public:
        VersionReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~VersionReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // myserver_VersionReactor
