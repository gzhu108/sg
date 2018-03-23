#pragma once
#ifndef streetgangworldapi_WorldResponder
#define streetgangworldapi_WorldResponder

#include "MessageResponder.h"
#include "Rectangle.h"
#include "Point.h"
#include "WorldId.h"
#include "MessageBase.h"


namespace streetgangworldapi
{
    class WorldResponder : public sg::microreactor::MessageResponder
    {
    public:
        WorldResponder(sg::microreactor::Connection& connection);
        virtual ~WorldResponder();

    public:
        virtual bool SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage);
        virtual bool SendCreateWorldResponse(const std::string& trackId, ResultCode result, const streetgangworldapi::WorldId& worldId, const std::string& worldName);
        virtual bool SendGetWorldResponse(const std::string& trackId, ResultCode result, const streetgangworldapi::WorldId& worldId, const streetgangworldapi::Rectangle<float>& rect, const std::vector<streetgangworldapi::Point<float>>& items);
    
    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message);
    };
}


#endif // streetgangworldapi_WorldResponder
