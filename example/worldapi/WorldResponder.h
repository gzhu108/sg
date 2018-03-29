#pragma once
#ifndef worldapi_WorldResponder
#define worldapi_WorldResponder

#include "MessageResponder.h"
#include "Rectangle.h"
#include "Point.h"
#include "WorldId.h"
#include "MessageBase.h"


namespace worldapi
{
    class WorldResponder : public sg::microreactor::MessageResponder
    {
    public:
        WorldResponder(sg::microreactor::Connection& connection);
        virtual ~WorldResponder();

    public:
        virtual bool SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage);
        virtual bool SendCreateWorldResponse(const std::string& trackId, ResultCode result, const worldapi::WorldId& worldId, const std::string& worldName);
        virtual bool SendGetWorldResponse(const std::string& trackId, ResultCode result, const worldapi::WorldId& worldId, const worldapi::Rectangle<float>& rect, const std::vector<worldapi::Point<float>>& items);
    
    protected:
        virtual bool SendMessage(std::shared_ptr<MessageBase> message);
    };
}


#endif // worldapi_WorldResponder
