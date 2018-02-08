#pragma once
#ifndef streetgangserver_StreetGangMessageDecoder
#define streetgangserver_StreetGangMessageDecoder

#include "MessageDecoder.h"
#include "BinaryStreetGangResponder.h"


namespace streetgangserver
{
    class StreetGangMessageDecoder : public sg::microreactor::MessageDecoder
    {
    public:
        StreetGangMessageDecoder();
        virtual ~StreetGangMessageDecoder();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<streetgangapi::BinaryStreetGangResponder> mResponder;
    };
}

#endif // streetgangserver_StreetGangMessageDecoder
