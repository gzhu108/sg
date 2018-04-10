#pragma once
#ifndef bouncer_BouncerReactor
#define bouncer_BouncerReactor

#include <vector>
#include "Reactor.h"


namespace bouncer
{
    class BouncerReactor : public sg::microreactor::Reactor
    {
    public:
        BouncerReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<sg::microreactor::Connection> target, std::istream& stream);
        virtual ~BouncerReactor();

    public:
        virtual bool Process() override;

    protected:
        bool ReceiveTarget();

    protected:
        std::shared_ptr<sg::microreactor::Connection> mTarget;
        std::vector<char> mData;
    };
}


#endif // bouncer_BouncerReactor
