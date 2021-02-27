#pragma once
#ifndef microreactor_MessageDispatcher
#define microreactor_MessageDispatcher

#include "Dispatcher.h"
#include "Connection.h"
#include "Reactor.h"


namespace microreactor
{
    class MessageDispatcher : public Dispatcher
    {
    public:
        MessageDispatcher();
        virtual ~MessageDispatcher();

    public:
        virtual void Dispatch(Connection& connection) override;

    protected:
        virtual std::shared_ptr<Reactor> Decode(std::istream& stream, Connection& connection) = 0;

    protected:
        std::stringstream mReceiveStream;
    };
}


#endif // microreactor_MessageDispatcher
