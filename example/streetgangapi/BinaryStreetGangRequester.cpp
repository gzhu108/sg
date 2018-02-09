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


BinaryStreetGangRequester::BinaryStreetGangRequester(Connection& connection)
    : StreetGangRequester(connection)
{
}

BinaryStreetGangRequester::~BinaryStreetGangRequester()
{
}

bool BinaryStreetGangRequester::Byebye()
{
    auto message = std::make_shared<BinaryRequestByebye>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(message);
}

bool BinaryStreetGangRequester::GetVersion()
{
    auto message = std::make_shared<BinaryRequestGetVersion>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(message);
}

bool BinaryStreetGangRequester::CreateWorld(const std::string& worldName)
{
    auto message = std::make_shared<RequestCreateWorld>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldName.set(worldName);
    return SendMessage(message);
}

bool BinaryStreetGangRequester::GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect)
{
    auto message = std::make_shared<RequestGetScene>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    return SendMessage(message);
}

bool BinaryStreetGangRequester::SendMessage(std::shared_ptr<MessageBase> message)
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
