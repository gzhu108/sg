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
        PBStreetGangRequester(microreactor::Connection& connection);
        virtual ~PBStreetGangRequester();

    public:
        virtual bool Byebye(std::shared_ptr<microreactor::Reactor> client) override;
        virtual bool GetVersion(std::shared_ptr<microreactor::Reactor> client) override;
        virtual bool CreateWorld(const std::string& worldName, std::shared_ptr<microreactor::Reactor> client) override;
        virtual bool GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect, std::shared_ptr<microreactor::Reactor> client) override;

    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message, std::shared_ptr<microreactor::Reactor> client);
   };
}

#endif // streetgangapi_PBStreetGangRequester
