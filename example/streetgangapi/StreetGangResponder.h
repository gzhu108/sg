#pragma once
#ifndef streetgangapi_StreetGangResponder
#define streetgangapi_StreetGangResponder

#include "MessageResponder.h"
#include "Rectangle.h"
#include "Point.h"
#include "SessionId.h"
#include "StreetGangIds.h"


namespace streetgangapi
{
    class StreetGangResponder : public sg::microreactor::MessageResponder
    {
    public:
        StreetGangResponder(std::shared_ptr<sg::microreactor::Connection> connection) : sg::microreactor::MessageResponder(connection) {}
        virtual ~StreetGangResponder() {}

    public:
        virtual bool SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage) = 0;
        virtual bool SendGetVersionResponse(const std::string& trackId, ResultCode result, const std::string& version) = 0;
        virtual bool SendCreateWorldResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName) = 0;
        virtual bool SendGetSceneResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items) = 0;
    };
}


#endif // streetgangapi_StreetGangResponder
