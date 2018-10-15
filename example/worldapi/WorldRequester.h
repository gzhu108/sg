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
    class WorldRequester : protected sg::microreactor::MessageRequester
    {
    public:
        WorldRequester(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<WorldCache> worldCache = nullptr);
        virtual ~WorldRequester();

    public:
        virtual std::shared_ptr<ResponseCreateWorld> CreateWorld(const std::string& worldName, std::shared_ptr<sg::microreactor::Reactor> reactor = nullptr);
        virtual std::shared_ptr<ResponseGetWorld> GetWorld(const WorldId& worldId, std::shared_ptr<sg::microreactor::Reactor> reactor = nullptr);

    protected:
        virtual bool SendMessage(std::shared_ptr<WorldMessage> message);

    protected:
        std::shared_ptr<WorldCache> mWorldCache;
   };
}

#endif // worldapi_WorldRequester
