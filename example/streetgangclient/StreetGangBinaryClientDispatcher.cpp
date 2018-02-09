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

#define CREATE_RESPONSE_REACTOR(_id, _message, _reactor) \
    case (int32_t)(_id): \
    { \
        auto message = std::make_shared<_message>(); \
        if (message->Decode(stream)) \
        { \
            auto reactor = std::make_shared<_reactor>(connection, message, mRequester); \
            return reactor; \
        } \
    } \
    break

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


StreetGangBinaryClientDispatcher::StreetGangBinaryClientDispatcher()
{
    mRequester = std::make_shared<streetgangapi::BinaryStreetGangRequester>();
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

    switch (id)
    {
        CREATE_RESPONSE_REACTOR(streetgangapi::ID::Error, BinaryResponseError, ResponseErrorReactor);
        CREATE_RESPONSE_REACTOR(streetgangapi::ID::GetVersionResponse, BinaryResponseGetVersion, ResponseGetVersionReactor);
        CREATE_RESPONSE_REACTOR(streetgangapi::ID::CreateWorldResponse, BinaryResponseCreateWorld, ResponseCreateWorldReactor);
        CREATE_RESPONSE_REACTOR(streetgangapi::ID::GetSceneResponse, BinaryResponseGetScene, ResponseGetSceneReactor);

        default:
        {
            /*auto message = std::make_shared<ResponseError>();
            message->Result.set((int32_t)ResultCode::ErrorNotImplemented);
            message->RequestId.set(id);
            message->ErrorMessage.set("Unknown Request");
            auto reactor = std::make_shared<RequestErrorReactor>(connection, message);
            reactor->SetMessageEncoder(mStreetGangMessageResponseEncoder);
            return reactor;*/
        }
    }

    LOG("[" FMT_INT64 "] StreetGangBinaryClientDispatcher::Decode() [ID=%d] [Error=Failed to decode message]",
        std::chrono::high_resolution_clock::now().time_since_epoch().count(),
        id);

    return nullptr;
}
