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
        reactor.SetParent(message);
    }
    
    return true;
}

void Dispatcher::RegisterMessage(std::shared_ptr<Message> message)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    
    // Only register the message that expects a response
    if (message != nullptr && message->ResponseTimeout.cref() > 0)
    {
        // Track the message
        mTrackedMessages[message->TrackId.cref()] = message;
        message->SetRequestTime();
    }
}

void Dispatcher::RemoveTimedOutMessages(Connection& connection)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    
    for (auto& message : mTrackedMessages)
    {
        if (message.second != nullptr && message.second->HasTimedOut())
        {
            LOG("[" FMT_INT64 "] Dispatcher::RemoveTimedOutMessages() [TrackId=%s]",
            std::chrono::high_resolution_clock::now().time_since_epoch().count(),
            message.second->TrackId->c_str());

            mMessageTimedOut(TimedOutMessage(message.second, connection));
            message.second = nullptr;
        }
    }
}

std::shared_ptr<Message> Dispatcher::GetTrackedMessage(const std::string& trackId)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Find tracked message by track ID
    auto found = mTrackedMessages.find(trackId);
    if (found != mTrackedMessages.end())
    {
        std::shared_ptr<Message> trackedMessage = found->second;
        mTrackedMessages.erase(found);
        return trackedMessage;
    }

    return nullptr;
}
