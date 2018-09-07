#pragma once
#ifndef streetgangserver_WorldClient
#define streetgangserver_WorldClient

#include "Client.h"


namespace streetgangserver
{
    class WorldClient : public sg::microreactor::Client
    {
    public:
        virtual ~WorldClient();
        static WorldClient& GetInstance();
        static void ResetWorldClient();

    protected:
        WorldClient(const std::string& protocol, const std::string& hostAddress, uint16_t port);
        static std::shared_ptr<WorldClient> mInstance;
   };
}

#endif // streetgangserver_StreetGangClient
