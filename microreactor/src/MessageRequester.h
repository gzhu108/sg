#pragma once
#ifndef sg_microreactor_MessageRequester
#define sg_microreactor_MessageRequester

#include "Message.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    class MessageRequester
    {
    public:
        explicit MessageRequester(std::shared_ptr<Connection> connection);
        virtual ~MessageRequester();

    public:
        virtual bool SendMessage(std::shared_ptr<Message> message, std::shared_ptr<Reactor> client);

    protected:
        std::shared_ptr<Connection> mConnection;
   };
}}

#endif // sg_microreactor_MessageRequester
