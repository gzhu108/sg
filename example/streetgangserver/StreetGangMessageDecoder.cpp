#include "StreetGangMessageDecoder.h"
#include "StreetGangIds.h"
#include "StreetGangSessionManager.h"

#include "RequestErrorReactor.h"
#include "RequestGetVersionReactor.h"
#include "RequestByebyeReactor.h"
#include "RequestCreateWorldReactor.h"
#include "RequestGetSceneReactor.h"

#define CREATE_REQUEST_REACTOR(_id, _message, _reactor) \
    case (int32_t)(_id): \
    { \
        auto message = std::make_shared<_message>(); \
        if (DecodeMessage(stream, *message)) \
        { \
            auto reactor = std::make_shared<_reactor>(connection, message); \
            reactor->SetMessageEncoder(mStreetGangMessageResponseEncoder); \
            return reactor; \
        } \
    } \
    break

#define CREATE_SESSION_REQUEST_REACTOR(_id, _message, _reactor) \
    case (int32_t)(_id): \
    { \
        auto message = std::make_shared<_message>(); \
        if (DecodeMessage(stream, *message)) \
        { \
            auto reactor = std::make_shared<_reactor>(connection, message); \
            reactor->SetMessageEncoder(mStreetGangMessageResponseEncoder); \
            auto session = StreetGangSessionManager::GetInstance().GetSession(message->WorldId.cref()); \
            reactor->SetSession(session); \
            return reactor; \
        } \
    } \
    break

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


StreetGangMessageDecoder::StreetGangMessageDecoder()
{
    mStreetGangMessageResponseEncoder = std::make_shared<StreetGangMessageResponseEncoder>();
}

StreetGangMessageDecoder::~StreetGangMessageDecoder()
{
}

std::shared_ptr<Reactor> StreetGangMessageDecoder::Decode(std::istream& stream, Connection& connection)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);
    
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

    switch (id)
    {
        CREATE_REQUEST_REACTOR(streetgangapi::ID::Byebye, RequestByebye, RequestByebyeReactor);
        CREATE_REQUEST_REACTOR(streetgangapi::ID::GetVersionRequest, RequestGetVersion, RequestGetVersionReactor);
        CREATE_REQUEST_REACTOR(streetgangapi::ID::CreateWorldRequest, RequestCreateWorld, RequestCreateWorldReactor);
        CREATE_SESSION_REQUEST_REACTOR(streetgangapi::ID::GetSceneRequest, RequestGetScene, RequestGetSceneReactor);
        
        default:
        {
            auto message = std::make_shared<ResponseError>();
            message->Result.set((int32_t)ResultCode::ErrorNotImplemented);
            message->RequestId.set(id);
            message->ErrorMessage.set("Unknown Request");
            auto reactor = std::make_shared<RequestErrorReactor>(connection, message);
            reactor->SetMessageEncoder(mStreetGangMessageResponseEncoder);
            return reactor;
        }
    }

    return nullptr;
}

bool StreetGangMessageDecoder::DecodeMessage(std::istream& stream, sg::microreactor::Message& message)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    std::string trackId;
    if (serializer.Read(stream, trackId))
    {
        message.TrackId.set(trackId);
    }
    else
    {
        return false;
    }

    uint64_t responseTimeout = 0;
    if (serializer.Read(stream, responseTimeout))
    {
        message.ResponseTimeout.set(responseTimeout);
    }
    else
    {
        return false;
    }

    return true;
}

bool StreetGangMessageDecoder::DecodeMessage(std::istream& stream, MessageBase& message)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    // Do not read the ID, it should be read by the receiver.

    if (!DecodeMessage(stream, static_cast<Message&>(message)))
    {
        return false;
    }

    int32_t result = static_cast<int32_t>(ResultCode::Unknown);
    if (serializer.Read(stream, result))
    {
        message.Result.set(result);
    }
    else
    {
        return false;
    }

    SessionId sessionId;
    if (serializer.Read(stream, sessionId))
    {
        message.MessageSessionId.set(sessionId);
    }
    else
    {
        return false;
    }

    return true;
}

bool StreetGangMessageDecoder::DecodeMessage(std::istream& stream, RequestByebye& message)
{
    return DecodeMessage(stream, static_cast<MessageBase&>(message));
}

bool StreetGangMessageDecoder::DecodeMessage(std::istream& stream, RequestGetVersion& message)
{
    return DecodeMessage(stream, static_cast<MessageBase&>(message));
}

bool StreetGangMessageDecoder::DecodeMessage(std::istream& stream, RequestCreateWorld& message)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    if (!DecodeMessage(stream, static_cast<MessageBase&>(message)))
    {
        return false;
    }

    std::string worldName;
    if (serializer.Read(stream, worldName))
    {
        message.WorldName.set(worldName);
    }
    else
    {
        return false;
    }

    return true;
}

bool StreetGangMessageDecoder::DecodeMessage(std::istream& stream, RequestGetScene& message)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    if (!DecodeMessage(stream, static_cast<MessageBase&>(message)))
    {
        return false;
    }

    SessionId worldId = DefaultSessionId;
    if (serializer.Read(stream, worldId))
    {
        message.WorldId.set(worldId);
    }
    else
    {
        return false;
    }

    auto rect = message.Rect.cref();
    if (serializer.Read(stream, rect))
    {
        if (rect.mX < 0.0 || rect.mX > 1.0)
        {
            LOG("Invalid Rect(%f, %f, %f, %f)",
                rect.mX,
                rect.mY,
                rect.mW,
                rect.mH);
        }

        message.Rect.set(rect);

        if (message.Rect->mX < 0.0 || message.Rect->mX > 1.0)
        {
            LOG("Invalid Message Rect(%f, %f, %f, %f)",
                message.Rect->mX,
                message.Rect->mY,
                message.Rect->mW,
                message.Rect->mH);
        }
    }
    else
    {
        return false;
    }

    return true;
}
