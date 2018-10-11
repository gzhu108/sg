#pragma once
#ifndef sg_microreactor_Dispatcher
#define sg_microreactor_Dispatcher

#include <unordered_map>
#include "Common.h"
#include "Park.h"
#include "Message.h"


namespace sg { namespace microreactor
{
    class Connection;
    class Reactor;

    class Dispatcher
    {
    public:
        Dispatcher();
        virtual ~Dispatcher();

        struct TimedOutMessage
        {
            TimedOutMessage(std::shared_ptr<Message> message, Connection& connection)
                : mMessage(message)
                , mConnection(connection)
            {
            }

            std::shared_ptr<Message> mMessage;
            Connection& mConnection;
        };

        Signal<TimedOutMessage>& MessageTimedOut = mMessageTimedOut;

    public:
        virtual void Dispatch(Connection& connection) = 0;
        virtual bool InitializeReactor(Reactor& reactor);
        virtual void RegisterMessage(std::shared_ptr<Message> message);
        virtual void RemoveTimedOutMessages(Connection& connection);

    protected:
        virtual std::shared_ptr<Message> GetTrackedMessage(const std::string& trackId);

    protected:
        Emittable<TimedOutMessage> mMessageTimedOut;
        Park<Message::ParkingSpaceNumber> mTrackedMessages;
    };
}}


#endif // sg_microreactor_Dispatcher
