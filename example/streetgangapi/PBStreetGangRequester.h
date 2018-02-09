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
        PBStreetGangRequester(sg::microreactor::Connection& connection);
        virtual ~PBStreetGangRequester();

    public:
        virtual bool Byebye() override;
        virtual bool GetVersion() override;
        virtual bool CreateWorld(const std::string& worldName) override;
        virtual bool GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect) override;

    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message);
   };
}

#endif // streetgangapi_PBStreetGangRequester
