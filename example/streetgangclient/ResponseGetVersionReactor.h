#pragma once
#ifndef streetgangclient_ResponseGetVersionReactor
#define streetgangclient_ResponseGetVersionReactor

#include "MessageReactor.h"
#include "StreetGangRequestEncoder.h"
#include "ResponseGetVersion.h"


namespace streetgangclient
{
    class ResponseGetVersionReactor : public sg::microreactor::MessageReactor<streetgangapi::ResponseGetVersion, streetgangapi::StreetGangRequestEncoder>
    {
    public:
        ResponseGetVersionReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseGetVersion> message);
        virtual ~ResponseGetVersionReactor();

    public:
        virtual bool Process() override;

    protected:
        void SendNextRequest();
    };
}


#endif // streetgangclient_ResponseGetVersionReactor
