#pragma once
#ifndef streetgangapi_StreetGangResponder
#define streetgangapi_StreetGangResponder

#include "Connection.h"
#include "Rectangle.h"
#include "Point.h"
#include "SessionId.h"
#include "StreetGangIds.h"


namespace streetgangapi
{
    class StreetGangResponder
    {
    public:
        virtual bool SendErrorResponse(sg::microreactor::Connection& connection, const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage) = 0;
        virtual bool SendGetVersionResponse(sg::microreactor::Connection& connection, const std::string& trackId, ResultCode result, const std::string& version) = 0;
        virtual bool SendCreateWorldResponse(sg::microreactor::Connection& connection, const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName) = 0;
        virtual bool SendGetSceneResponse(sg::microreactor::Connection& connection, const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items) = 0;
    };
}


#endif // streetgangapi_StreetGangResponder
