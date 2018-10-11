#pragma once
#ifndef sg_microreactor_Reactor
#define sg_microreactor_Reactor

#include "Parkable.h"
#include "Message.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    class Reactor : public Parkable<uintptr_t>
    {
    public:
        explicit Reactor(std::shared_ptr<Connection> connection);
        virtual ~Reactor();

    public:
        virtual bool Process() = 0;
        virtual void SetOriginalMessage(std::shared_ptr<Message> message) { mOriginalMessage = message; }
        virtual std::shared_ptr<Message> Input() { return mInput; }

    protected:
        std::shared_ptr<Connection> mConnection;
        std::shared_ptr<Message> mOriginalMessage;
        std::shared_ptr<Message> mInput;
    };
}}


#endif // sg_microreactor_Reactor
