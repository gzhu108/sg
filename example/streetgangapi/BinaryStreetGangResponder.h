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
        BinaryStreetGangResponder(std::shared_ptr<sg::microreactor::Connection> connection);
        virtual ~BinaryStreetGangResponder();

    public:
        virtual bool SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage) override;
        virtual bool SendGetVersionResponse(const std::string& trackId, ResultCode result, const std::string& version) override;
        virtual bool SendCreateWorldResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName) override;
        virtual bool SendGetSceneResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items) override;

    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message);
    };
}


#endif // streetgangapi_BinaryStreetGangResponder
