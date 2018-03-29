#pragma once
#ifndef worldapi_StreetGangRequester
#define worldapi_StreetGangRequester

#include "MessageRequester.h"
#include "WorldId.h"
#include "WorldMessage.h"


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
        virtual bool SendMessage(std::shared_ptr<WorldMessage> message);
   };
}

#endif // worldapi_StreetGangRequester
