#pragma once
#ifndef bouncer_BouncerReactor
#define bouncer_BouncerReactor

#include <vector>
#include "Reactor.h"


namespace bouncer
{
    class BouncerReactor : public microreactor::Reactor
    {
    public:
        BouncerReactor(microreactor::Connection& connection, std::shared_ptr<microreactor::Connection> target, std::istream& stream);
        virtual ~BouncerReactor();

    public:
        virtual bool Process() override;

    protected:
        bool ReceiveTarget();

    protected:
        std::shared_ptr<microreactor::Connection> mTarget;
        std::vector<char> mData;
    };
}


#endif // bouncer_BouncerReactor
