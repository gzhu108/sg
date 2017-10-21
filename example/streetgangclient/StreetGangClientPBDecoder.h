#pragma once
#ifndef streetgangclient_StreetGangClientPBDecoder
#define streetgangclient_StreetGangClientPBDecoder

#include "MessageDecoder.h"
#include "StreetGangPBRequestEncoder.h"


namespace streetgangclient
{
    class StreetGangClientPBDecoder : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangClientPBDecoder();
        virtual ~StreetGangClientPBDecoder();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangPBRequestEncoder> mStreetGangPBRequestEncoder;
    };
}

#endif // streetgangclient_StreetGangClientPBDecoder
