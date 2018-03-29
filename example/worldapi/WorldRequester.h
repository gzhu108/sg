#pragma once
#ifndef worldapi_StreetGangRequester
#define worldapi_StreetGangRequester

#include "MessageRequester.h"
#include "WorldId.h"
#include "MessageBase.h"


namespace worldapi
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

#endif // worldapi_StreetGangRequester
