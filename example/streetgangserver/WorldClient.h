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
        virtual ~WorldClient();
        static WorldClient& GetInstance();

    protected:
        WorldClient(const std::string& protocol, const std::string& hostName, uint16_t port);
        static std::shared_ptr<WorldClient> mInstance;
   };
}

#endif // streetgangserver_StreetGangClient
