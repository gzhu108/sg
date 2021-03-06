#pragma once
#ifndef streetgangclient_ResponseGetVersionReactor
#define streetgangclient_ResponseGetVersionReactor

#include "MessageReactor.h"
#include "StreetGangRequester.h"
#include "ResponseGetVersion.h"


namespace streetgangclient
{
    class ResponseGetVersionReactor : public microreactor::Reactor
    {
    public:
        ResponseGetVersionReactor(microreactor::Connection& connection, std::shared_ptr<streetgangapi::StreetGangRequester> requester);
        virtual ~ResponseGetVersionReactor();

    public:
        virtual bool ProcessError(std::shared_ptr<microreactor::Message> errorMessage) override;
        virtual bool ProcessTimeout(std::shared_ptr<microreactor::Message> timedOutMessage) override;

        bool Process(std::shared_ptr<streetgangapi::ResponseGetVersion> response);

    protected:
        void SendNextRequest();

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequester> mRequester;
    };
}


#endif // streetgangclient_ResponseGetVersionReactor
