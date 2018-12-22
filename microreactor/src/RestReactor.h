#pragma once
#ifndef sg_microreactor_RestReactor
#define sg_microreactor_RestReactor

#include "Reactor.h"
#include "Connection.h"
#include "RestMessage.h"
#include "Uuid.h"


namespace sg { namespace microreactor
{
    template <typename T>
    class RestReactor : public Reactor
    {
    public:
        typedef T MessageType;

        RestReactor(Connection& connection, std::shared_ptr<MessageType> request)
            : Reactor(connection)
        {
            mInput = std::static_pointer_cast<Message>(request);
            mInput->TrackId.set(Uuid::GenerateUuid().ToString());
        }

        virtual ~RestReactor() {}

    protected:
        virtual std::shared_ptr<MessageType> InputMessage()
        {
            return std::static_pointer_cast<MessageType>(mInput);
        }
    };
}}


#endif // sg_microreactor_RestReactor
