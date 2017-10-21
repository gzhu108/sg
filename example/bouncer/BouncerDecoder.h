#pragma once
#ifndef bouncer_BouncerDecoder
#define bouncer_BouncerDecoder

#include "MessageDecoder.h"


namespace bouncer
{
    class BouncerDecoder : public sg::microreactor::MessageDecoder
    {
    public:
        BouncerDecoder();
        virtual ~BouncerDecoder();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;
    };
}

#endif // bouncer_BouncerDecoder
