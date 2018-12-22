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
        explicit MessageRequester(Connection& connection);
        virtual ~MessageRequester();

    public:
        virtual bool SendMessage(std::shared_ptr<Message> message, std::shared_ptr<Reactor> client);

    protected:
        Connection& mConnection;
   };
}}

#endif // sg_microreactor_MessageRequester
