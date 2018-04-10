#pragma once
#ifndef sg_microreactor_MessageReactor
#define sg_microreactor_MessageReactor

#include "Reactor.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    template <typename T>
    class MessageReactor : public Reactor
    {
    public:
        typedef T MessageType;

        MessageReactor(std::shared_ptr<Connection> connection, std::shared_ptr<MessageType> input)
            : Reactor(connection)
        {
            mInput = std::static_pointer_cast<Message>(input);
        }

        virtual ~MessageReactor() {}

    public:
        virtual std::shared_ptr<MessageType> InputMessage()
        {
            return std::static_pointer_cast<MessageType>(mInput);
        }
    };
}}


#endif // sg_microreactor_MessageReactor
