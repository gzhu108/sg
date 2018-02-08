#include "BinaryStreetGangRequester.h"
#include "BinarySerializer.h"
#include "Uuid.h"
#include "StreetGangIds.h"

#include "BinaryRequestByebye.h"
#include "BinaryRequestGetVersion.h"
#include "BinaryRequestCreateWorld.h"
#include "BinaryRequestGetScene.h"
#include "BinaryResponseError.h"

using namespace sg::microreactor;
using namespace streetgangapi;


BinaryStreetGangRequester::BinaryStreetGangRequester()
{
}

BinaryStreetGangRequester::~BinaryStreetGangRequester()
{
}

bool BinaryStreetGangRequester::Byebye(Connection& connection)
{
    auto message = std::make_shared<BinaryRequestByebye>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(connection, message);
}

bool BinaryStreetGangRequester::GetVersion(Connection& connection)
{
    auto message = std::make_shared<BinaryRequestGetVersion>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(connection, message);
}

bool BinaryStreetGangRequester::CreateWorld(Connection& connection, const std::string& worldName)
{
    auto message = std::make_shared<RequestCreateWorld>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldName.set(worldName);
    return SendMessage(connection, message);
}

bool BinaryStreetGangRequester::GetScene(Connection& connection, const SessionId& worldId, const streetgangapi::Rectangle<float>& rect)
{
    auto message = std::make_shared<RequestGetScene>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    return SendMessage(connection, message);
}

bool BinaryStreetGangRequester::SendMessage(sg::microreactor::Connection& connection, std::shared_ptr<MessageBase> message)
{
    if (message == nullptr || connection.IsClosed())
    {
        auto peerName = connection.GetPeerName();
        auto peerPort = connection.GetPeerPort();
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
            uint64_t sent = connection.Send(stream);
            if (sent > 0)
            {
                // Register the message with the dispatcher
                connection.RegisterMessage(message);
                return true;
            }
        }
    }

    auto peerName = connection.GetPeerName();
    auto peerPort = connection.GetPeerPort();
    LOG("[Message=%d] [TrackId=%s] Fail to send response [Connection=%s:%u]", message->Id.cref(), message->TrackId->c_str(), peerName.c_str(), peerPort);
    return false;
}
