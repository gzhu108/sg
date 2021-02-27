#include "WorldDispatcher.h"
#include "WorldManager.h"
#include "BinarySerializer.h"

#include "CreateWorldReactor.h"
#include "GetWorldReactor.h"

#include "RequestCreateWorld.h"
#include "RequestGetWorld.h"

using namespace microreactor;
using namespace worldapi;
using namespace worldserver;


WorldDispatcher::WorldDispatcher()
{
    RegisterReactorFactory(static_cast<int32_t>(worldapi::ID::CreateWorldRequest), std::bind(&WorldDispatcher::CreateCreateWorldReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterReactorFactory(static_cast<int32_t>(worldapi::ID::GetWorldRequest), std::bind(&WorldDispatcher::CreateGetWorldReactor, this, std::placeholders::_1, std::placeholders::_2));
}

WorldDispatcher::~WorldDispatcher()
{
}

std::shared_ptr<Reactor> WorldDispatcher::Decode(std::istream& stream, Connection& connection)
{
    BinarySerializer serializer;
    
    int32_t id = (int32_t)ID::Unknown;
    if (!serializer.Read(stream, id))
    {
        // pushback the total number of characters in ID
        int64_t bytes = stream.gcount();
        auto position = (int64_t)stream.tellg() - bytes;
        stream.seekg(position, stream.beg);
        stream.clear();
        return nullptr;
    }

    uint64_t length = 0;
    if (!serializer.Read(stream, length) || GetStreamSize(stream) < length)
    {
        // pushback the total number of characters in ID + Length
        int64_t bytes = stream.gcount() + (std::streamsize)sizeof(int32_t);
        auto position = (int64_t)stream.tellg() - bytes;
        stream.seekg(position, stream.beg);
        stream.clear();
        return nullptr;
    }

    auto factory = GetReactorFactory(id);
    if (factory == nullptr)
    {
        auto responder = std::make_shared<WorldResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorNotImplemented, id, "Unknown request");
        return nullptr;
    }

    return factory(stream, connection);
}

std::shared_ptr<Reactor> WorldDispatcher::CreateCreateWorldReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<RequestCreateWorld>();
    if (message->Decode(stream))
    {
        return std::make_shared<CreateWorldReactor>(connection, message, std::make_shared<WorldResponder>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> WorldDispatcher::CreateGetWorldReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<RequestGetWorld>();
    if (message->Decode(stream))
    {
        return std::make_shared<GetWorldReactor>(connection, message, std::make_shared<WorldResponder>(connection));
    }
    
    return nullptr;
}
