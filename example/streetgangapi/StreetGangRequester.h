#pragma once
#ifndef streetgangapi_StreetGangRequester
#define streetgangapi_StreetGangRequester

#include "MessageRequester.h"
#include "SessionId.h"
#include "Rectangle.h"


namespace streetgangapi
{
    class StreetGangRequester : protected sg::microreactor::MessageRequester
    {
    public:
        StreetGangRequester(std::shared_ptr<sg::microreactor::Connection> connection) : sg::microreactor::MessageRequester(connection) {}
        virtual ~StreetGangRequester() {}

    public:
        virtual bool Byebye() = 0;
        virtual bool GetVersion() = 0;
        virtual bool CreateWorld(const std::string& worldName) = 0;
        virtual bool GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect) = 0;
   };
}

#endif // streetgangapi_StreetGangRequester
