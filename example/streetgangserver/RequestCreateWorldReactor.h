#pragma once
#ifndef streetgangserver_RequestCreateWorldReactor
#define streetgangserver_RequestCreateWorldReactor

#include "MessageReactor.h"
#include "StreetGangResponder.h"
#include "worldapi/ResponseCreateWorld.h"
#include "worldapi/ResponseError.h"
#include "streetgangapi/RequestCreateWorld.h"
#include "StreetGangReactor.h"


namespace streetgangserver
{
    class RequestCreateWorldReactor
        : public sg::microreactor::MessageReactor<streetgangapi::RequestCreateWorld>
        , public StreetGangReactor
    {
    public:
        RequestCreateWorldReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<streetgangapi::RequestCreateWorld> message, std::shared_ptr<streetgangapi::StreetGangResponder> responder);
        virtual ~RequestCreateWorldReactor();
        
    public:
        virtual bool Process() override;
        virtual bool ProcessError(std::shared_ptr<sg::microreactor::Message> errorMessage) override;
        virtual bool ProcessTimeout(std::shared_ptr<sg::microreactor::Message> timedOutMessage) override;

        bool ProcessCreateWorldResponse(std::shared_ptr<worldapi::ResponseCreateWorld> response);
        bool SendResponse(const streetgangapi::SessionId& sessionId, const std::string& worldName);
    };
}


#endif // streetgangserver_RequestCreateWorldReactor
