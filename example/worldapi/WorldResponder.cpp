#include "WorldResponder.h"
#include "BinarySerializer.h"
#include "ResponseError.h"
#include "ResponseCreateWorld.h"
#include "ResponseGetWorld.h"

using namespace sg::microreactor;
using namespace worldapi;


WorldResponder::WorldResponder(Connection& connection)
    : MessageResponder(connection)
{
}

WorldResponder::~WorldResponder()
{
}

bool WorldResponder::SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage)
{
    auto message = std::make_shared<ResponseError>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->RequestId.set(requestId);
    message->ErrorMessage.set(errorMessage);
    return SendMessage(message);
}

bool WorldResponder::SendCreateWorldResponse(const std::string& trackId, ResultCode result, const worldapi::WorldId& worldId, const std::string& worldName)
{
    auto message = std::make_shared<ResponseCreateWorld>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->WorldName.set(worldName);
    return SendMessage(message);
}

bool WorldResponder::SendGetWorldResponse(const std::string& trackId, ResultCode result, const worldapi::World& world)
{
    auto message = std::make_shared<ResponseGetWorld>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->World.set(world);
    return SendMessage(message);
}

bool WorldResponder::SendMessage(std::shared_ptr<WorldMessage> message)
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
            uint64_t sent = mConnection.Send(stream);
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
