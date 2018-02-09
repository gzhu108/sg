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
        BinaryStreetGangRequester(sg::microreactor::Connection& connection);
        virtual ~BinaryStreetGangRequester();

    public:
        virtual bool Byebye() override;
        virtual bool GetVersion() override;
        virtual bool CreateWorld(const std::string& worldName) override;
        virtual bool GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect) override;

    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message);
   };
}

#endif // streetgangapi_BinaryStreetGangRequester
