#pragma once
#ifndef streetgangclient_ResponseCreateWorldReactor
#define streetgangclient_ResponseCreateWorldReactor

#include "MessageReactor.h"
#include "StreetGangRequester.h"
#include "ResponseCreateWorld.h"


namespace streetgangclient
{
    class ResponseCreateWorldReactor : public microreactor::Reactor
    {
    public:
        ResponseCreateWorldReactor(microreactor::Connection& connection, std::shared_ptr<streetgangapi::StreetGangRequester> requester);
        virtual ~ResponseCreateWorldReactor();
        
    public:
        virtual bool ProcessError(std::shared_ptr<microreactor::Message> errorMessage) override;
        virtual bool ProcessTimeout(std::shared_ptr<microreactor::Message> timedOutMessage) override;

        bool Process(std::shared_ptr<streetgangapi::ResponseCreateWorld> response);

    protected:
        void SendNextRequest(uintptr_t worldId);

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequester> mRequester;
    };
}


#endif // streetgangclient_ResponseCreateWorldReactor
