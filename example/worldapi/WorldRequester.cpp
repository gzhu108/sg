#include "WorldRequester.h"
#include "BinarySerializer.h"
#include "Uuid.h"
#include "WorldId.h"

#include "RequestCreateWorld.h"
#include "RequestGetWorld.h"

using namespace microreactor;
using namespace worldapi;


WorldRequester::WorldRequester(Connection& connection, std::shared_ptr<WorldCache> worldCache)
    : MessageRequester(connection)
    , mWorldCache(worldCache)
{
}

WorldRequester::~WorldRequester()
{
}

std::shared_ptr<ResponseCreateWorld> WorldRequester::CreateWorld(const std::string& worldName, std::shared_ptr<Reactor> client)
{
    WorldId worldId;
    if (mWorldCache == nullptr || !mWorldCache->GetWorldId(worldName, worldId))
    {
        auto message = std::make_shared<RequestCreateWorld>();
        message->TrackId.set(Uuid::GenerateUuid().ToString());
        message->WorldName.set(worldName);
        SendMessage(message, client);
        return nullptr;
    }

    auto response = std::make_shared<ResponseCreateWorld>();
    response->SetRequestTime();
    response->Result.set((int32_t)ResultCode::Success);
    response->WorldId.set(worldId);
    response->WorldName.set(worldName);
    return response;
}

std::shared_ptr<ResponseGetWorld> WorldRequester::GetWorld(const WorldId& worldId, std::shared_ptr<Reactor> client)
{
    World world;
    if (mWorldCache == nullptr || !mWorldCache->GetWorld(worldId, world))
    {
        auto message = std::make_shared<RequestGetWorld>();
        message->TrackId.set(Uuid::GenerateUuid().ToString());
        message->WorldId.set(worldId);
        SendMessage(message, client);
        return nullptr;
    }

    auto response = std::make_shared<ResponseGetWorld>();
    response->SetRequestTime();
    response->Result.set((int32_t)ResultCode::Success);
    response->World.set(world);
    return response;
}

bool WorldRequester::SendMessage(std::shared_ptr<WorldMessage> message, std::shared_ptr<Reactor> client)
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
