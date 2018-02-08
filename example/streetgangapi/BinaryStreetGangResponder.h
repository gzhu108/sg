#pragma once
#ifndef streetgangapi_BinaryStreetGangResponder
#define streetgangapi_BinaryStreetGangResponder

#include "StreetGangResponder.h"
#include "MessageBase.h"


namespace streetgangapi
{
    class BinaryStreetGangResponder : public StreetGangResponder
    {
    public:
        BinaryStreetGangResponder();
        virtual ~BinaryStreetGangResponder();

    public:
        virtual bool SendErrorResponse(sg::microreactor::Connection& connection, const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage) override;
        virtual bool SendGetVersionResponse(sg::microreactor::Connection& connection, const std::string& trackId, ResultCode result, const std::string& version) override;
        virtual bool SendCreateWorldResponse(sg::microreactor::Connection& connection, const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName) override;
        virtual bool SendGetSceneResponse(sg::microreactor::Connection& connection, const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items) override;

    protected:
        virtual bool SendMessage(sg::microreactor::Connection& connection, std::shared_ptr<MessageBase> message);
    };
}


#endif // streetgangapi_BinaryStreetGangResponder
