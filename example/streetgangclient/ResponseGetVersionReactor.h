#pragma once
#ifndef streetgangclient_ResponseGetVersionReactor
#define streetgangclient_ResponseGetVersionReactor

#include "MessageReactor.h"
#include "StreetGangRequester.h"
#include "ResponseGetVersion.h"


namespace streetgangclient
{
    class ResponseGetVersionReactor : public sg::microreactor::MessageReactor<streetgangapi::ResponseGetVersion>
    {
    public:
        ResponseGetVersionReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseGetVersion> message, std::shared_ptr<streetgangapi::StreetGangRequester> requester);
        virtual ~ResponseGetVersionReactor();

    public:
        virtual bool Process() override;

    protected:
        void SendNextRequest();

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequester> mRequester;
    };
}


#endif // streetgangclient_ResponseGetVersionReactor
