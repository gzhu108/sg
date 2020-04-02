#pragma once
#ifndef sg_microreactor_RestReactor
#define sg_microreactor_RestReactor

#include "Reactor.h"
#include "Connection.h"
#include "RestRequest.h"
#include "RestResponse.h"
#include "Uuid.h"


namespace sg { namespace microreactor
{
    template <typename T = RestMessage>
    class RestReactor : public Reactor
    {
    public:
        typedef T MessageType;

        RestReactor(Connection& connection, std::shared_ptr<MessageType> input)
            : Reactor(connection, input)
        {
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
