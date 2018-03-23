#pragma once
#ifndef streetgangworldapi_StreetGangRequester
#define streetgangworldapi_StreetGangRequester

#include "MessageRequester.h"
#include "WorldId.h"
#include "MessageBase.h"


namespace streetgangworldapi
{
    class WorldRequester : protected sg::microreactor::MessageRequester
    {
    public:
        WorldRequester(sg::microreactor::Connection& connection);
        virtual ~WorldRequester();

    public:
        virtual bool CreateWorld(const std::string& worldName);
        virtual bool GetWorld(const WorldId& worldId);

    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message);
   };
}

#endif // streetgangworldapi_StreetGangRequester
