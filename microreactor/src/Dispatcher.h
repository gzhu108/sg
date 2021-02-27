#pragma once
#ifndef microreactor_Dispatcher
#define microreactor_Dispatcher

#include <unordered_map>
#include "Common.h"
#include "Park.h"
#include "Message.h"
#include "Profile.h"


namespace microreactor
{
    class Connection;
    class Reactor;

    class Dispatcher : public Profile
    {
    public:
        Dispatcher();
        virtual ~Dispatcher();

    public:
        virtual void Dispatch(Connection& connection);
        virtual void RegisterMessage(std::shared_ptr<Message> message);
        virtual void RemoveTimedOutMessages(Connection& connection);

    protected:
        virtual std::shared_ptr<Message> GetTrackedMessage(const std::string& trackId);

    protected:
        Park<Message::ParkingSpaceNumber> mTrackedMessages;
    };
}


#endif // microreactor_Dispatcher
