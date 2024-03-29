#include "Dispatcher.h"
#include "Connection.h"
#include "Reactor.h"

using namespace microreactor;


Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
}

void Dispatcher::Dispatch(Connection& connection)
{
    // Default implementation does nothing
}

void Dispatcher::RegisterMessage(std::shared_ptr<Message> message)
{
    // Only register the message that expects a response
    if (message != nullptr && message->ResponseTimeout() > std::chrono::milliseconds::zero())
    {
        // Track the message
        mTrackedMessages.Add(message->TrackId(), message);
        message->SetRequestTime();
    }
}

void Dispatcher::RemoveTimedOutMessages(Connection& connection)
{
    std::vector<std::shared_ptr<Parkable<Message::ParkingSpaceNumber>>> trackedMessages;
    if (mTrackedMessages.GetAll(trackedMessages))
    {
        for (auto& tracked : trackedMessages)
        {
            auto message = std::static_pointer_cast<Message>(tracked);
            if (message != nullptr && message->HasTimedOut())
            {
                LOG("[" FMT_INT64 "] Dispatcher::RemoveTimedOutMessages() [TrackId=%s]",
                    std::chrono::high_resolution_clock::now().time_since_epoch().count(),
                    message->TrackId->c_str());

                auto client = message->Client();
                if (client != nullptr)
                {
                    client->ProcessTimeout(message);
                }

                mTrackedMessages.Remove(message->TrackId());
            }
        }
    }
}

std::shared_ptr<Message> Dispatcher::GetTrackedMessage(const std::string& trackId)
{
    return std::static_pointer_cast<Message>(mTrackedMessages.Remove(trackId));
}
