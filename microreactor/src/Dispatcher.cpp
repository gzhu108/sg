#include "Dispatcher.h"
#include "Connection.h"
#include "Reactor.h"

using namespace sg::microreactor;


Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
}

bool Dispatcher::InitializeReactor(Reactor& reactor)
{
    if (reactor.Input() != nullptr)
    {
        auto message = GetTrackedMessage(reactor.Input()->TrackId.cref());
        reactor.SetOriginalMessage(message);
    }
    
    return true;
}

void Dispatcher::RegisterMessage(std::shared_ptr<Message> message)
{
    // Only register the message that expects a response
    if (message != nullptr && message->ResponseTimeout.cref() > 0)
    {
        // Track the message
        mTrackedMessages.Add(message->TrackId.cref(), message);
        message->SetRequestTime();
    }
}

void Dispatcher::RemoveTimedOutMessages(Connection& connection)
{
    std::vector<std::shared_ptr<Parkable<Message::ParkingSpaceNumber>>> trackedMessages;
    if (mTrackedMessages.GetAll(trackedMessages))
    {
        for (auto& trackedMessage : trackedMessages)
        {
            auto message = std::static_pointer_cast<Message>(trackedMessage);
            if (message != nullptr && message->HasTimedOut())
            {
                LOG("[" FMT_INT64 "] Dispatcher::RemoveTimedOutMessages() [TrackId=%s]",
                    std::chrono::high_resolution_clock::now().time_since_epoch().count(),
                    message->TrackId->c_str());

                mMessageTimedOut(TimedOutMessage(message, connection));
                mTrackedMessages.Remove(message->TrackId.cref());
            }
        }
    }
}

std::shared_ptr<Message> Dispatcher::GetTrackedMessage(const std::string& trackId)
{
    return std::static_pointer_cast<Message>(mTrackedMessages.Remove(trackId));
}
