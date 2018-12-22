#include "MessageRequester.h"

using namespace sg::microreactor;


MessageRequester::MessageRequester(Connection& connection)
    : mConnection(connection)
{
}

MessageRequester::~MessageRequester()
{
}

bool MessageRequester::SendMessage(std::shared_ptr<Message> message, std::shared_ptr<Reactor> client)
{
    if (message == nullptr || mConnection.IsClosed())
    {
        auto peerAddress = mConnection.GetPeerAddress();
        auto peerPort = mConnection.GetPeerPort();
        LOG("Fail to send response [Connection=%s:%u]", peerAddress.c_str(), peerPort);
        return false;
    }

    std::stringstream stream;
    if (message->Encode(stream))
    {
        // Send message
        uint64_t sent = mConnection.Send(stream);
        if (sent > 0)
        {
            // Register the message with the dispatcher
            mConnection.RegisterMessage(message, client);
            return true;
        }
    }

    auto peerAddress = mConnection.GetPeerAddress();
    auto peerPort = mConnection.GetPeerPort();
    LOG("[TrackId=%s] Fail to send response [Connection=%s:%u]", message->TrackId->c_str(), peerAddress.c_str(), peerPort);
    return false;
}
