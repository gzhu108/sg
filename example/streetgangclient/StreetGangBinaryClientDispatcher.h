#pragma once
#ifndef streetgangclient_StreetGangBinaryClientDispatcher
#define streetgangclient_StreetGangBinaryClientDispatcher

#include "MessageDecoder.h"
#include "BinaryStreetGangRequester.h"


namespace streetgangclient
{
    class StreetGangBinaryClientDispatcher : public sg::microreactor::MessageDecoder<int32_t>
    {
    public:
        StreetGangBinaryClientDispatcher();
        virtual ~StreetGangBinaryClientDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<streetgangapi::BinaryStreetGangRequester> mRequester;
    };
}

#endif // streetgangclient_StreetGangClientReactorFactory
