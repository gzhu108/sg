#pragma once
#ifndef streetgangclient_ResponseCreateWorldReactor
#define streetgangclient_ResponseCreateWorldReactor

#include "MessageReactor.h"
#include "StreetGangRequestEncoder.h"
#include "ResponseCreateWorld.h"


namespace streetgangclient
{
    class ResponseCreateWorldReactor
        : public sg::microreactor::MessageReactor<streetgangapi::ResponseCreateWorld, streetgangapi::StreetGangRequestEncoder>
        , public std::enable_shared_from_this<ResponseCreateWorldReactor>
    {
    public:
        ResponseCreateWorldReactor(sg::microreactor::Connection& connection, std::shared_ptr<streetgangapi::ResponseCreateWorld> message);
        virtual ~ResponseCreateWorldReactor();
        
    public:
        virtual bool Process() override;

    protected:
        void SendNextRequest();
    };
}


#endif // streetgangclient_ResponseCreateWorldReactor
