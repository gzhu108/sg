#pragma once
#ifndef sg_microreactor_MessageDecoder
#define sg_microreactor_MessageDecoder

#include "Dispatcher.h"


namespace sg { namespace microreactor
{
    class Connection;
    class Reactor;

    class MessageDecoder : public Dispatcher
    {
    public:
        MessageDecoder();
        virtual ~MessageDecoder();

    public:
        virtual void Dispatch(Connection& connection) override;

    protected:
        virtual std::shared_ptr<Reactor> Decode(std::istream& stream, Connection& connection) = 0;

    protected:
        std::stringstream mReceiveStream;
    };
}}


#endif // sg_microreactor_MessageDecoder
