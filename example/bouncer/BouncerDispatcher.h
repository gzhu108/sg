#pragma once
#ifndef bouncer_BouncerDecoder
#define bouncer_BouncerDecoder

#include "MessageDispatcher.h"


namespace bouncer
{
    class BouncerDispatcher : public microreactor::MessageDispatcher
    {
    public:
        BouncerDispatcher();
        virtual ~BouncerDispatcher();

        PROPERTY(TargetName, std::string);
        PROPERTY(TargetPort, uint16_t);

    public:
        virtual std::shared_ptr<microreactor::Reactor> Decode(std::istream& stream, microreactor::Connection& connection) override;
    };
}

#endif // bouncer_BouncerDecoder
