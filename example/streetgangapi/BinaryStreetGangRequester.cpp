#include "BinaryStreetGangRequester.h"
#include "BinarySerializer.h"
#include "Uuid.h"
#include "StreetGangIds.h"

#include "BinaryRequestByebye.h"
#include "BinaryRequestGetVersion.h"
#include "BinaryRequestCreateWorld.h"
#include "BinaryRequestGetScene.h"
#include "BinaryResponseError.h"

using namespace microreactor;
using namespace streetgangapi;


BinaryStreetGangRequester::BinaryStreetGangRequester(Connection& connection)
    : StreetGangRequester(connection)
{
}

BinaryStreetGangRequester::~BinaryStreetGangRequester()
{
}

bool BinaryStreetGangRequester::Byebye(std::shared_ptr<Reactor> client)
{
    auto message = std::make_shared<BinaryRequestByebye>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(message, client);
}

bool BinaryStreetGangRequester::GetVersion(std::shared_ptr<Reactor> client)
{
    auto message = std::make_shared<BinaryRequestGetVersion>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(message, client);
}

bool BinaryStreetGangRequester::CreateWorld(const std::string& worldName, std::shared_ptr<Reactor> client)
{
    auto message = std::make_shared<BinaryRequestCreateWorld>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldName.set(worldName);
    return SendMessage(message, client);
}

bool BinaryStreetGangRequester::GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect, std::shared_ptr<Reactor> client)
{
    auto message = std::make_shared<BinaryRequestGetScene>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    return SendMessage(message, client);
}

bool BinaryStreetGangRequester::SendMessage(std::shared_ptr<MessageBase> message, std::shared_ptr<Reactor> client)
{
    if (message == nullptr || mConnection.IsClosed())
    {
        auto peerAddress = mConnection.GetPeerAddress();
        auto peerPort = mConnection.GetPeerPort();
        LOG("Fail to send response [Connection=%s:%u]", peerAddress.c_str(), peerPort);
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
            uint64_t sent = mConnection.Write(stream);
            if (sent > 0)
            {
                // Register the message with the dispatcher
                mConnection.RegisterMessage(message, client);
                return true;
            }
        }
    }

    auto peerAddress = mConnection.GetPeerAddress();
    auto peerPort = mConnection.GetPeerPort();
    LOG("[Message=%d] [TrackId=%s] Fail to send response [Connection=%s:%u]", message->Id.cref(), message->TrackId->c_str(), peerAddress.c_str(), peerPort);
    return false;
}
