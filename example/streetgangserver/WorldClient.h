#pragma once
#ifndef streetgangserver_WorldClient
#define streetgangserver_WorldClient

#include "Client.h"
#include "StreetGangReactor.h"


namespace streetgangserver
{
    class WorldClient : public sg::microreactor::Client
    {
    public:
        WorldClient(const std::string& protocol, const std::string& hostName, uint16_t port, std::shared_ptr<StreetGangReactor> reactor);
        virtual ~WorldClient();

    protected:
        std::shared_ptr<StreetGangReactor> mReactor;
   };
}

#endif // streetgangserver_StreetGangClient
