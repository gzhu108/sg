#pragma once
#ifndef streetgangapi_StreetGangRequester
#define streetgangapi_StreetGangRequester

#include "Connection.h"
#include "SessionId.h"
#include "Rectangle.h"


namespace streetgangapi
{
    class StreetGangRequester
    {
    public:
        virtual bool Byebye(sg::microreactor::Connection& connection) = 0;
        virtual bool GetVersion(sg::microreactor::Connection& connection) = 0;
        virtual bool CreateWorld(sg::microreactor::Connection& connection, const std::string& worldName) = 0;
        virtual bool GetScene(sg::microreactor::Connection& connection, const SessionId& worldId, const streetgangapi::Rectangle<float>& rect) = 0;
   };
}

#endif // streetgangapi_StreetGangRequester
