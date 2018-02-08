#pragma once
#ifndef streetgangserver_StreetGangBinaryDispatcher
#define streetgangserver_StreetGangBinaryDispatcher

#include "MessageDecoder.h"
#include "BinaryStreetGangResponder.h"


namespace streetgangserver
{
    class StreetGangBinaryDispatcher : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangBinaryDispatcher();
        virtual ~StreetGangBinaryDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<streetgangapi::BinaryStreetGangResponder> mResponder;
    };
}

#endif // streetgangserver_StreetGangBinaryDispatcher
