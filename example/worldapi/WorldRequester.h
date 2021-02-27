#pragma once
#ifndef worldapi_WorldRequester
#define worldapi_WorldRequester

#include "MessageRequester.h"
#include "Reactor.h"
#include "WorldCache.h"
#include "ResponseCreateWorld.h"
#include "ResponseGetWorld.h"


namespace worldapi
{
    class WorldRequester : protected microreactor::MessageRequester
    {
    public:
        WorldRequester(microreactor::Connection& connection, std::shared_ptr<WorldCache> worldCache = nullptr);
        virtual ~WorldRequester();

    public:
        virtual std::shared_ptr<ResponseCreateWorld> CreateWorld(const std::string& worldName, std::shared_ptr<microreactor::Reactor> client);
        virtual std::shared_ptr<ResponseGetWorld> GetWorld(const WorldId& worldId, std::shared_ptr<microreactor::Reactor> client);

    protected:
        virtual bool SendMessage(std::shared_ptr<WorldMessage> message, std::shared_ptr<microreactor::Reactor> client);

    protected:
        std::shared_ptr<WorldCache> mWorldCache;
   };
}

#endif // worldapi_WorldRequester
