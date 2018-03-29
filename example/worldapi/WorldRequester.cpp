#include "WorldRequester.h"
#include "BinarySerializer.h"
#include "Uuid.h"
#include "WorldId.h"

#include "RequestCreateWorld.h"
#include "RequestGetWorld.h"

using namespace sg::microreactor;
using namespace worldapi;


WorldRequester::WorldRequester(Connection& connection)
    : MessageRequester(connection)
{
}

WorldRequester::~WorldRequester()
{
}

bool WorldRequester::CreateWorld(const std::string& worldName, uintptr_t userData)
{
    auto message = std::make_shared<RequestCreateWorld>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->UserData.set(userData);
    message->WorldName.set(worldName);
    return SendMessage(message);
}

bool WorldRequester::GetWorld(const WorldId& worldId, uintptr_t userData)
{
    auto message = std::make_shared<RequestGetWorld>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->UserData.set(userData);
    message->WorldId.set(worldId);

    return SendMessage(message);
}

bool WorldRequester::SendMessage(std::shared_ptr<WorldMessage> message)
{
    if (message == nullptr || mConnection.IsClosed())
    {
        auto peerName = mConnection.GetPeerName();
        auto peerPort = mConnection.GetPeerPort();
        LOG("Fail to send response [Connection=%s:%u]", peerName.c_str(), peerPort);
        return false;
    }

    std::stringstream messageStream;
    if (message->Encode(messageStream))
    {
        BinarySerializer serializer;
        std::stringstream stream;
        uint64_t length = GetStreamSize(messageStream);
        if (serializer.Write(message->Id.cref(), stream) &&
            serializer.Write(length, stream) &&
            serializer.Write(messageStream, stream))
        {
            // Send message
            uint64_t sent = mConnection.Send(stream);
            if (sent > 0)
            {
                // Register the message with the dispatcher
                mConnection.RegisterMessage(message);
                return true;
            }
        }
    }

    auto peerName = mConnection.GetPeerName();
    auto peerPort = mConnection.GetPeerPort();
    LOG("[Message=%d] [TrackId=%s] Fail to send response [Connection=%s:%u]", message->Id.cref(), message->TrackId->c_str(), peerName.c_str(), peerPort);
    return false;
}
