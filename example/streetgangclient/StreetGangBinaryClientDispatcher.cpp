#include "StreetGangBinaryClientDispatcher.h"
#include "BinarySerializer.h"

#include "ResponseErrorReactor.h"
#include "ResponseGetVersionReactor.h"
#include "ResponseCreateWorldReactor.h"
#include "ResponseGetSceneReactor.h"

#include "BinaryResponseError.h"
#include "BinaryResponseGetVersion.h"
#include "BinaryResponseCreateWorld.h"
#include "BinaryResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


StreetGangBinaryClientDispatcher::StreetGangBinaryClientDispatcher()
{
    RegisterMessageReactorFactory(static_cast<int32_t>(streetgangapi::ID::Error), std::bind(&StreetGangBinaryClientDispatcher::CreateErrorResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory(static_cast<int32_t>(streetgangapi::ID::GetVersionResponse), std::bind(&StreetGangBinaryClientDispatcher::CreateGetVersionResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory(static_cast<int32_t>(streetgangapi::ID::CreateWorldResponse), std::bind(&StreetGangBinaryClientDispatcher::CreateCreateWorldResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory(static_cast<int32_t>(streetgangapi::ID::GetSceneResponse), std::bind(&StreetGangBinaryClientDispatcher::CreateGetSceneResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
}

StreetGangBinaryClientDispatcher::~StreetGangBinaryClientDispatcher()
{
}

std::shared_ptr<Reactor> StreetGangBinaryClientDispatcher::Decode(std::istream& stream, Connection& connection)
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
        LOG("[" FMT_INT64 "] StreetGangBinaryClientDispatcher::Decode() [ID=%d] [Error=Failed to decode message]",
            std::chrono::high_resolution_clock::now().time_since_epoch().count(),
            id);

        return nullptr;
    }

    return factory(stream, std::static_pointer_cast<Connection>(connection.shared_from_this()));
}

std::shared_ptr<Reactor> StreetGangBinaryClientDispatcher::CreateErrorResponseReactor(std::istream& stream, std::shared_ptr<Connection> connection)
{
    auto message = std::make_shared<BinaryResponseError>();
    if (message->Decode(stream))
    {
        return std::make_shared<ResponseErrorReactor>(connection, message, std::make_shared<streetgangapi::BinaryStreetGangRequester>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangBinaryClientDispatcher::CreateGetVersionResponseReactor(std::istream& stream, std::shared_ptr<Connection> connection)
{
    auto message = std::make_shared<BinaryResponseGetVersion>();
    if (message->Decode(stream))
    {
        return std::make_shared<ResponseGetVersionReactor>(connection, message, std::make_shared<streetgangapi::BinaryStreetGangRequester>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangBinaryClientDispatcher::CreateCreateWorldResponseReactor(std::istream& stream, std::shared_ptr<Connection> connection)
{
    auto message = std::make_shared<BinaryResponseCreateWorld>();
    if (message->Decode(stream))
    {
        return std::make_shared<ResponseCreateWorldReactor>(connection, message, std::make_shared<streetgangapi::BinaryStreetGangRequester>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangBinaryClientDispatcher::CreateGetSceneResponseReactor(std::istream& stream, std::shared_ptr<Connection> connection)
{
    auto message = std::make_shared<BinaryResponseGetScene>();
    if (message->Decode(stream))
    {
        return std::make_shared<ResponseGetSceneReactor>(connection, message, std::make_shared<streetgangapi::BinaryStreetGangRequester>(connection));
    }
    
    return nullptr;
}
