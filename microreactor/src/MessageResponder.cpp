#include "MessageResponder.h"

using namespace sg::microreactor;


MessageResponder::MessageResponder(std::shared_ptr<Connection> connection)
    : mConnection(connection)
{
}

MessageResponder::~MessageResponder()
{
}

bool MessageResponder::SendMessage(std::shared_ptr<Message> message)
{
    if (message == nullptr || mConnection == nullptr || mConnection->IsClosed())
    {
        auto peerName = mConnection->GetPeerName();
        auto peerPort = mConnection->GetPeerPort();
        LOG("Fail to send response [Connection=%s:%u]", peerName.c_str(), peerPort);
        return false;
    }

    std::stringstream stream;
    if (message->Encode(stream))
    {
        // Send message
        uint64_t sent = mConnection->Send(stream);
        if (sent > 0)
        {
            return true;
        }
    }

    auto peerName = mConnection->GetPeerName();
    auto peerPort = mConnection->GetPeerPort();
    LOG("[TrackId=%s] Fail to send response [Connection=%s:%u]", message->TrackId->c_str(), peerName.c_str(), peerPort);
    return false;
}
