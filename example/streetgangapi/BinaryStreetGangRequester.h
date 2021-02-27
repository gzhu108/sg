#pragma once
#ifndef streetgangapi_BinaryStreetGangRequester
#define streetgangapi_BinaryStreetGangRequester

#include "StreetGangRequester.h"
#include "MessageBase.h"


namespace streetgangapi
{
    class BinaryStreetGangRequester : public StreetGangRequester
    {
    public:
        BinaryStreetGangRequester(microreactor::Connection& connection);
        virtual ~BinaryStreetGangRequester();

    public:
        virtual bool Byebye(std::shared_ptr<microreactor::Reactor> client) override;
        virtual bool GetVersion(std::shared_ptr<microreactor::Reactor> client) override;
        virtual bool CreateWorld(const std::string& worldName, std::shared_ptr<microreactor::Reactor> client) override;
        virtual bool GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect, std::shared_ptr<microreactor::Reactor> client) override;

    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message, std::shared_ptr<microreactor::Reactor> client);
   };
}

#endif // streetgangapi_BinaryStreetGangRequester
