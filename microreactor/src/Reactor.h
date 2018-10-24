#pragma once
#ifndef sg_microreactor_Reactor
#define sg_microreactor_Reactor

#include "Parkable.h"
#include "Message.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    class Reactor : public Parkable<std::string>
    {
    public:
        explicit Reactor(std::shared_ptr<Connection> connection);
        virtual ~Reactor();

    public:
        virtual bool Process() { return false; }
        virtual bool ProcessError(std::shared_ptr<Message> errorMessage) { return false; }
        virtual bool ProcessTimeout(std::shared_ptr<Message> timedOutMessage) { return false; }
        virtual std::shared_ptr<Message> Input() { return mInput; }

    protected:
        std::shared_ptr<Connection> mConnection;
        std::shared_ptr<Message> mInput;
    };
}}


#endif // sg_microreactor_Reactor
