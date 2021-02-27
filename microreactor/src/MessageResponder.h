#pragma once
#ifndef microreactor_MessageResponder
#define microreactor_MessageResponder

#include "Message.h"
#include "Connection.h"


namespace microreactor
{
    class MessageResponder
    {
    public:
        explicit MessageResponder(Connection& connection);
        virtual ~MessageResponder();

    public:
        virtual bool SendMessage(std::shared_ptr<Message> message);

    protected:
        Connection& mConnection;
    };
}


#endif // microreactor_MessageResponder
