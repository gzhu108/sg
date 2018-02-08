#pragma once
#ifndef streetgangclient_StreetGangPBClientDispatcher
#define streetgangclient_StreetGangPBClientDispatcher

#include "MessageDecoder.h"
#include "StreetGangPBRequestEncoder.h"


namespace streetgangclient
{
    class StreetGangPBClientDispatcher : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangPBClientDispatcher();
        virtual ~StreetGangPBClientDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<streetgangapi::StreetGangPBRequestEncoder> mStreetGangPBRequestEncoder;
    };
}

#endif // streetgangclient_StreetGangPBClientDispatcher
