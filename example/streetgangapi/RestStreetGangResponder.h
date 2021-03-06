#pragma once
#ifndef streetgangapi_RestStreetGangResponder
#define streetgangapi_RestStreetGangResponder

#include "StreetGangResponder.h"
#include "MessageBase.h"


namespace streetgangapi
{
    class RestStreetGangResponder : public StreetGangResponder
    {
    public:
        RestStreetGangResponder(microreactor::Connection& connection);
        virtual ~RestStreetGangResponder();

    public:
        virtual bool SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage) override;
        virtual bool SendGetVersionResponse(const std::string& trackId, ResultCode result, const std::string& version) override;
        virtual bool SendCreateWorldResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName) override;
        virtual bool SendGetSceneResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items) override;

    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message);
    };
}


#endif // streetgangapi_RestStreetGangResponder
