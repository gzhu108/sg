#pragma once
#ifndef streetgangclient_StreetGangClientMessageDecoder
#define streetgangclient_StreetGangClientMessageDecoder

#include "MessageDecoder.h"


namespace streetgangclient
{
    class StreetGangClientMessageDecoder : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangClientMessageDecoder();
        virtual ~StreetGangClientMessageDecoder();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;
    };
}

#endif // streetgangclient_StreetGangClientReactorFactory
