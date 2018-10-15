#include "WorldRequester.h"
#include "BinarySerializer.h"
#include "Uuid.h"
#include "WorldId.h"

#include "RequestCreateWorld.h"
#include "RequestGetWorld.h"

using namespace sg::microreactor;
using namespace worldapi;


WorldRequester::WorldRequester(std::shared_ptr<Connection> connection, std::shared_ptr<WorldCache> worldCache)
    : MessageRequester(connection)
    , mWorldCache(worldCache)
{
}

WorldRequester::~WorldRequester()
{
}

std::shared_ptr<ResponseCreateWorld> WorldRequester::CreateWorld(const std::string& worldName, std::shared_ptr<Reactor> reactor)
{
    WorldId worldId;
    if (mWorldCache == nullptr || !mWorldCache->GetWorldId(worldName, worldId))
    {
        auto message = std::make_shared<RequestCreateWorld>();
        message->TrackId.set(Uuid::GenerateUuid().ToString());
        message->OriginalReactor.set(reactor);
        message->WorldName.set(worldName);
        SendMessage(message);
        return nullptr;
    }

    auto response = std::make_shared<ResponseCreateWorld>();
    response->WorldId.set(worldId);
    response->WorldName.set(worldName);
    return response;
}

std::shared_ptr<ResponseGetWorld> WorldRequester::GetWorld(const WorldId& worldId, std::shared_ptr<Reactor> reactor)
{
    World world;
    if (mWorldCache == nullptr || !mWorldCache->GetWorld(worldId, world))
    {
        auto message = std::make_shared<RequestGetWorld>();
        message->TrackId.set(Uuid::GenerateUuid().ToString());
        message->OriginalReactor.set(reactor);
        message->WorldId.set(worldId);
        SendMessage(message);
        return nullptr;
    }

    auto response = std::make_shared<ResponseGetWorld>();
    response->World.set(world);
    return response;
}

bool WorldRequester::SendMessage(std::shared_ptr<WorldMessage> message)
{
    if (message == nullptr || mConnection == nullptr || mConnection->IsClosed())
    {
        auto peerAddress = mConnection->GetPeerAddress();
        auto peerPort = mConnection->GetPeerPort();
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
            uint64_t sent = mConnection->Send(stream);
            if (sent > 0)
            {
                // Register the message with the dispatcher
                mConnection->RegisterMessage(message);
                return true;
            }
        }
    }

    auto peerAddress = mConnection->GetPeerAddress();
    auto peerPort = mConnection->GetPeerPort();
    LOG("[Message=%d] [TrackId=%s] Fail to send response [Connection=%s:%u]", message->Id.cref(), message->TrackId->c_str(), peerAddress.c_str(), peerPort);
    return false;
}
