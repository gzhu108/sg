#pragma once
#ifndef bouncer_BouncerDecoder
#define bouncer_BouncerDecoder

#include "MessageDispatcher.h"


namespace bouncer
{
    class BouncerDispatcher : public sg::microreactor::MessageDispatcher
    {
    public:
        BouncerDispatcher();
        virtual ~BouncerDispatcher();

        PROPERTY(TargetName, std::string);
        PROPERTY(TargetPort, uint16_t);

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;
    };
}

#endif // bouncer_BouncerDecoder
