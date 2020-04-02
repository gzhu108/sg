#pragma once
#ifndef sg_microreactor_MessageDispatcher
#define sg_microreactor_MessageDispatcher

#include "Dispatcher.h"
#include "Connection.h"
#include "Reactor.h"


namespace sg { namespace microreactor
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
}}


#endif // sg_microreactor_MessageDispatcher
