#pragma once
#ifndef streetgangapi_PBStreetGangRequester
#define streetgangapi_PBStreetGangRequester

#include "StreetGangRequester.h"
#include "MessageBase.h"


namespace streetgangapi
{
    class PBStreetGangRequester : public StreetGangRequester
    {
    public:
        PBStreetGangRequester();
        virtual ~PBStreetGangRequester();

    public:
        virtual bool Byebye(sg::microreactor::Connection& connection) override;
        virtual bool GetVersion(sg::microreactor::Connection& connection) override;
        virtual bool CreateWorld(sg::microreactor::Connection& connection, const std::string& worldName) override;
        virtual bool GetScene(sg::microreactor::Connection& connection, const SessionId& worldId, const streetgangapi::Rectangle<float>& rect) override;

    protected:
        bool SendMessage(sg::microreactor::Connection& connection, std::shared_ptr<MessageBase> message);
   };
}

#endif // streetgangapi_PBStreetGangRequester
