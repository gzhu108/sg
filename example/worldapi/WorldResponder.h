#pragma once
#ifndef worldapi_WorldResponder
#define worldapi_WorldResponder

#include "MessageResponder.h"
#include "World.h"
#include "WorldMessage.h"


namespace worldapi
{
    class WorldResponder : public sg::microreactor::MessageResponder
    {
    public:
        WorldResponder(std::shared_ptr<sg::microreactor::Connection> connection);
        virtual ~WorldResponder();

    public:
        virtual bool SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage);
        virtual bool SendCreateWorldResponse(const std::string& trackId, ResultCode result, const worldapi::WorldId& worldId, const std::string& worldName);
        virtual bool SendGetWorldResponse(const std::string& trackId, ResultCode result, const worldapi::World& world);
    
    protected:
        virtual bool SendMessage(std::shared_ptr<WorldMessage> message);
    };
}


#endif // worldapi_WorldResponder
