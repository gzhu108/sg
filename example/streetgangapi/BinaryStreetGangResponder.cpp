#include "BinaryStreetGangResponder.h"
#include "BinarySerializer.h"
#include "BinaryResponseError.h"
#include "BinaryResponseGetVersion.h"
#include "BinaryResponseCreateWorld.h"
#include "BinaryResponseGetScene.h"

using namespace microreactor;
using namespace streetgangapi;


BinaryStreetGangResponder::BinaryStreetGangResponder(Connection& connection)
    : StreetGangResponder(connection)
{
}

BinaryStreetGangResponder::~BinaryStreetGangResponder()
{
}

bool BinaryStreetGangResponder::SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage)
{
    auto message = std::make_shared<BinaryResponseError>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->RequestId.set(requestId);
    message->ErrorMessage.set(errorMessage);
    return SendMessage(message);
}

bool BinaryStreetGangResponder::SendGetVersionResponse(const std::string& trackId, ResultCode result, const std::string& version)
{
    auto message = std::make_shared<BinaryResponseGetVersion>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->Version.set(version);
    return SendMessage(message);
}

bool BinaryStreetGangResponder::SendCreateWorldResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName)
{
    auto message = std::make_shared<BinaryResponseCreateWorld>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->WorldName.set(worldName);
    return SendMessage(message);
}

bool BinaryStreetGangResponder::SendGetSceneResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items)
{
    auto message = std::make_shared<BinaryResponseGetScene>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    message->Items.set(items);
    return SendMessage(message);
}

bool BinaryStreetGangResponder::SendMessage(std::shared_ptr<MessageBase> message)
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
                return true;
            }
        }
    }

    auto peerAddress = mConnection.GetPeerAddress();
    auto peerPort = mConnection.GetPeerPort();
    LOG("[Message=%d] [TrackId=%s] Fail to send response [Connection=%s:%u]", message->Id.cref(), message->TrackId->c_str(), peerAddress.c_str(), peerPort);
    return false;
}
