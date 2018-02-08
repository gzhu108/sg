#pragma once
#ifndef streetgangclient_ResponseCreateWorldReactor
#define streetgangclient_ResponseCreateWorldReactor

#include "MessageReactor.h"
#include "StreetGangRequestEncoder.h"
#include "StreetGangRequester.h"
#include "ResponseCreateWorld.h"


namespace streetgangclient
{
    class ResponseCreateWorldReactor : public sg::microreactor::MessageReactor<streetgangapi::ResponseCreateWorld, streetgangapi::StreetGangRequestEncoder>
    {
    public:
        ResponseCreateWorldReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseCreateWorld> message, std::shared_ptr<streetgangapi::StreetGangRequester> requester);
        virtual ~ResponseCreateWorldReactor();
        
    public:
        virtual bool Process() override;

    protected:
        void SendNextRequest();

    protected:
        std::shared_ptr<streetgangapi::StreetGangRequester> mRequester;
    };
}


#endif // streetgangclient_ResponseCreateWorldReactor
