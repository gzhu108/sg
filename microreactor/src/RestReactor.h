#pragma once
#ifndef sg_microreactor_RestReactor
#define sg_microreactor_RestReactor

#include "Reactor.h"
#include "Connection.h"
#include "RestRequest.h"
#include "Uuid.h"


namespace sg { namespace microreactor
{
    template <typename T>
    class RestReactor : public Reactor
    {
    public:
        typedef T MessageType;

        RestReactor(Connection& connection, std::shared_ptr<RestRequest> request, std::shared_ptr<MessageType> input)
            : Reactor(connection)
            , mRestRequest(request)
        {
            mInput = std::static_pointer_cast<Message>(input);
            mInput->TrackId.set(Uuid::GenerateUuid().ToString());
        }

        virtual ~RestReactor() {}

    public:
        virtual std::shared_ptr<RestRequest> Request()
        {
            return mRestRequest;
        }

        virtual std::shared_ptr<MessageType> InputMessage()
        {
            return std::static_pointer_cast<MessageType>(mInput);
        }

    protected:
        std::shared_ptr<RestRequest> mRestRequest;
    };
}}


#endif // sg_microreactor_RestReactor
