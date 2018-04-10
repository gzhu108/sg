#pragma once
#ifndef sg_microreactor_Reactor
#define sg_microreactor_Reactor

#include "Parkable.h"
#include "Message.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    class Reactor : public Parkable
    {
    public:
        explicit Reactor(std::shared_ptr<Connection> connection);
        virtual ~Reactor();

    public:
        virtual bool Process() = 0;
        virtual void SetRequesterMessage(std::shared_ptr<Message> message) { mRequesterMessage = message; }
        virtual std::shared_ptr<Message> Input() { return mInput; }

    protected:
        std::shared_ptr<Connection> mConnection;
        std::shared_ptr<Message> mRequesterMessage;
        std::shared_ptr<Message> mInput;
    };
}}


#endif // sg_microreactor_Reactor
