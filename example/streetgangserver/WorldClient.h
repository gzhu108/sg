#pragma once
#ifndef streetgangserver_WorldClient
#define streetgangserver_WorldClient

#include "Client.h"
#include "WorldCache.h"


namespace streetgangserver
{
    class WorldClient : public microreactor::Client
    {
    public:
        virtual ~WorldClient();
        static WorldClient& GetInstance();
        static void ResetWorldClient();

        std::shared_ptr<worldapi::WorldCache> GetWorldCache() const { return mWorldCache; }

    protected:
        WorldClient(const std::string& protocol, const std::string& hostAddress, uint16_t port);

    protected:
        static std::shared_ptr<WorldClient> mInstance;
        std::shared_ptr<worldapi::WorldCache> mWorldCache;
   };
}

#endif // streetgangserver_StreetGangClient
