#pragma once
#ifndef sg_microreactor_Reactor
#define sg_microreactor_Reactor

#include "Message.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    class Reactor
    {
    public:
        explicit Reactor(Connection& connection);
        virtual ~Reactor();

    public:
        virtual bool Process() = 0;
        virtual void SetParent(std::shared_ptr<Message> message) { mParentMessage = message; }
        virtual std::shared_ptr<Message> Input() { return mInput; }

    protected:
        Connection& mConnection;
        std::shared_ptr<Message> mParentMessage;
        std::shared_ptr<Message> mInput;
    };
}}


#endif // sg_microreactor_Reactor
