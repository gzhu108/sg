#include "StreetGangBinaryDispatcher.h"
#include "BinarySerializer.h"

#include "RequestGetVersionReactor.h"
#include "RequestByebyeReactor.h"
#include "RequestCreateWorldReactor.h"
#include "RequestGetSceneReactor.h"

#include "BinaryRequestByebye.h"
#include "BinaryRequestGetVersion.h"
#include "BinaryRequestCreateWorld.h"
#include "BinaryRequestGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


StreetGangBinaryDispatcher::StreetGangBinaryDispatcher()
{
    RegisterReactorFactory(static_cast<int32_t>(streetgangapi::ID::Byebye), std::bind(&StreetGangBinaryDispatcher::CreateByebyeReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterReactorFactory(static_cast<int32_t>(streetgangapi::ID::GetVersionRequest), std::bind(&StreetGangBinaryDispatcher::CreateGetVersionReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterReactorFactory(static_cast<int32_t>(streetgangapi::ID::CreateWorldRequest), std::bind(&StreetGangBinaryDispatcher::CreateCreateWorldReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterReactorFactory(static_cast<int32_t>(streetgangapi::ID::GetSceneRequest), std::bind(&StreetGangBinaryDispatcher::CreateGetSceneReactor, this, std::placeholders::_1, std::placeholders::_2));
}

StreetGangBinaryDispatcher::~StreetGangBinaryDispatcher()
{
}

std::shared_ptr<Reactor> StreetGangBinaryDispatcher::Decode(std::istream& stream, Connection& connection)
{
    BinarySerializer serializer;
    
    //stream->clear();
    //stream->ignore(std::numeric_limits<std::streamsize>::max(), MARKER);
    //if (stream->eof() || stream->fail() || stream->bad())
    //{
    //    return nullptr;
    //}

    int32_t id = (int32_t)ID::Unknown;
    if (!serializer.Read(stream, id))
    {
        // pushback the total number of characters in ID
        auto bytes = stream.gcount();
        auto position = stream.tellg() - bytes;
        stream.seekg(position, stream.beg);
        stream.clear();
        return nullptr;
    }

    uint64_t length = 0;
    if (!serializer.Read(stream, length) || GetStreamSize(stream) < length)
    {
        // pushback the total number of characters in ID + Length
        auto bytes = stream.gcount() + (std::streamsize)sizeof(int32_t);
        auto position = stream.tellg() - bytes;
        stream.seekg(position, stream.beg);
        stream.clear();
        return nullptr;
    }

    auto factory = GetReactorFactory(id);
    if (factory == nullptr)
    {
        auto responder = std::make_shared<BinaryStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorNotImplemented, id, "Unknown request");
        return nullptr;
    }

    return factory(stream, connection);
}

std::shared_ptr<Reactor> StreetGangBinaryDispatcher::CreateByebyeReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<BinaryRequestByebye>();
    if (message->Decode(stream))
    {
        return std::make_shared<RequestByebyeReactor>(connection, message, std::make_shared<BinaryStreetGangResponder>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangBinaryDispatcher::CreateGetVersionReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<BinaryRequestGetVersion>();
    if (message->Decode(stream))
    {
        return std::make_shared<RequestGetVersionReactor>(connection, message, std::make_shared<BinaryStreetGangResponder>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangBinaryDispatcher::CreateCreateWorldReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<BinaryRequestCreateWorld>();
    if (message->Decode(stream))
    {
        return std::make_shared<RequestCreateWorldReactor>(connection, message, std::make_shared<BinaryStreetGangResponder>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangBinaryDispatcher::CreateGetSceneReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<BinaryRequestGetScene>();
    if (message->Decode(stream))
    {
        return std::make_shared<RequestGetSceneReactor>(connection, message, std::make_shared<BinaryStreetGangResponder>(connection));
    }
    
    return nullptr;
}
