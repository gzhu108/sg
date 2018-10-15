#include "WorldClientDispatcher.h"
#include "BinarySerializer.h"
#include "WorldClient.h"

#include "ResponseErrorReactor.h"
#include "ResponseCreateWorldReactor.h"
#include "ResponseGetWorldReactor.h"

#include "worldapi/ResponseError.h"
#include "worldapi/ResponseCreateWorld.h"
#include "worldapi/ResponseGetWorld.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


WorldClientDispatcher::WorldClientDispatcher()
{
    RegisterMessageReactorFactory(static_cast<int32_t>(worldapi::ID::Error), std::bind(&WorldClientDispatcher::CreateErrorResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory(static_cast<int32_t>(worldapi::ID::CreateWorldResponse), std::bind(&WorldClientDispatcher::CreateCreateWorldResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory(static_cast<int32_t>(worldapi::ID::GetWorldResponse), std::bind(&WorldClientDispatcher::CreateGetWorldResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
}

WorldClientDispatcher::~WorldClientDispatcher()
{
}

std::shared_ptr<Reactor> WorldClientDispatcher::Decode(std::istream& stream, Connection& connection)
{
    BinarySerializer serializer;

    int32_t id = (int32_t)ID::Unknown;
    if (!serializer.Read(stream, id))
    {
        // unget the total number of characters in ID
        auto bytes = stream.gcount();
        stream.clear();

        while (bytes)
        {
            stream.unget();
            bytes--;
        }

        return nullptr;
    }

    uint64_t length = 0;
    if (!serializer.Read(stream, length) || GetStreamSize(stream) < length)
    {
        // unget the total number of characters in ID + Length
        auto bytes = stream.gcount() + sizeof(int32_t);
        stream.clear();

        while (bytes)
        {
            stream.unget();
            if (stream.eof() || stream.fail() || stream.bad())
            {
                break;
            }

            bytes--;
        }

        return nullptr;
    }

    auto factory = GetMessageReactorFactory(id);
    if (factory == nullptr)
    {
        LOG("[" FMT_INT64 "] WorldClientDispatcher::Decode() [ID=%d] [Error=Failed to decode message]",
            std::chrono::high_resolution_clock::now().time_since_epoch().count(),
            id);

        return nullptr;
    }

    return factory(stream, std::static_pointer_cast<Connection>(connection.shared_from_this()));
}

std::shared_ptr<Reactor> WorldClientDispatcher::CreateErrorResponseReactor(std::istream& stream, std::shared_ptr<Connection> connection)
{
    auto message = std::make_shared<ResponseError>();
    if (message->Decode(stream))
    {
        return std::make_shared<ResponseErrorReactor>(connection, message);
    }

    return nullptr;
}

std::shared_ptr<Reactor> WorldClientDispatcher::CreateCreateWorldResponseReactor(std::istream& stream, std::shared_ptr<Connection> connection)
{
    auto message = std::make_shared<ResponseCreateWorld>();
    if (message->Decode(stream))
    {
        if (WorldClient::GetInstance().GetWorldCache() != nullptr)
        {
            WorldClient::GetInstance().GetWorldCache()->AddWorldId(message->WorldName.cref(), message->WorldId.cref());
        }

        return std::make_shared<ResponseCreateWorldReactor>(connection, message);
    }

    return nullptr;
}

std::shared_ptr<Reactor> WorldClientDispatcher::CreateGetWorldResponseReactor(std::istream& stream, std::shared_ptr<Connection> connection)
{
    auto message = std::make_shared<ResponseGetWorld>();
    if (message->Decode(stream))
    {
        if (WorldClient::GetInstance().GetWorldCache() != nullptr)
        {
            WorldClient::GetInstance().GetWorldCache()->AddWorld(message->World->mId, message->World.cref());
        }

        return std::make_shared<ResponseGetWorldReactor>(connection, message);
    }
    
    return nullptr;
}
