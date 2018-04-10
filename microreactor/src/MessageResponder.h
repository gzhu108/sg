#pragma once
#ifndef sg_microreactor_MessageResponder
#define sg_microreactor_MessageResponder

#include "Message.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    class MessageResponder
    {
    public:
        explicit MessageResponder(std::shared_ptr<Connection> connection);
        virtual ~MessageResponder();

    public:
        virtual bool SendMessage(std::shared_ptr<Message> message);

    protected:
        std::shared_ptr<Connection> mConnection;
    };
}}


#endif // sg_microreactor_MessageResponder
