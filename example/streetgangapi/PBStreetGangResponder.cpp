#include "PBStreetGangResponder.h"
#include "BinarySerializer.h"
#include "PBResponseError.h"
#include "PBResponseGetVersion.h"
#include "PBResponseCreateWorld.h"
#include "PBResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBStreetGangResponder::PBStreetGangResponder()
{
}

PBStreetGangResponder::~PBStreetGangResponder()
{
}

bool PBStreetGangResponder::SendErrorResponse(Connection& connection, const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage)
{
    auto message = std::make_shared<PBResponseError>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->RequestId.set(requestId);
    message->ErrorMessage.set(errorMessage);
    return SendMessage(connection, message);
}

bool PBStreetGangResponder::SendGetVersionResponse(Connection& connection, const std::string& trackId, ResultCode result, const std::string& version)
{
    auto message = std::make_shared<PBResponseGetVersion>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->Version.set(version);
    return SendMessage(connection, message);
}

bool PBStreetGangResponder::SendCreateWorldResponse(Connection& connection, const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName)
{
    auto message = std::make_shared<PBResponseCreateWorld>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->WorldName.set(worldName);
    return SendMessage(connection, message);
}

bool PBStreetGangResponder::SendGetSceneResponse(Connection& connection, const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items)
{
    auto message = std::make_shared<PBResponseGetScene>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    message->Items.set(items);
    return SendMessage(connection, message);
}

bool PBStreetGangResponder::SendMessage(Connection& connection, std::shared_ptr<MessageBase> message)
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
                return true;
            }
        }
    }

    auto peerName = connection.GetPeerName();
    auto peerPort = connection.GetPeerPort();
    LOG("[Message=%d] [TrackId=%s] Fail to send response [Connection=%s:%u]", message->Id.cref(), message->TrackId->c_str(), peerName.c_str(), peerPort);
    return false;
}
