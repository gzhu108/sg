#pragma once
#ifndef microreactor_MessageReactor
#define microreactor_MessageReactor

#include "Reactor.h"
#include "Connection.h"


namespace microreactor
{
    template <typename T = Message>
    class MessageReactor : public Reactor
    {
    public:
        typedef T MessageType;

        MessageReactor(Connection& connection, std::shared_ptr<MessageType> input)
            : Reactor(connection, std::static_pointer_cast<Message>(input))
        {
        }

        virtual ~MessageReactor() {}

    public:
        virtual std::shared_ptr<MessageType> InputMessage()
        {
            return std::static_pointer_cast<MessageType>(mInput);
        }
    };
}


#endif // microreactor_MessageReactor
