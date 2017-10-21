#include "StreetGangClientMessageDecoder.h"
#include "StreetGangIds.h"

#include "ResponseErrorReactor.h"
#include "ResponseGetVersionReactor.h"
#include "ResponseCreateWorldReactor.h"
#include "ResponseGetSceneReactor.h"

#define CREATE_RESPONSE_REACTOR(_id, _message, _reactor) \
    case (int32_t)(_id): \
    { \
        auto message = std::make_shared<_message>(); \
        if (DecodeMessage(stream, *message)) \
        { \
            auto reactor = std::make_shared<_reactor>(connection, message); \
            reactor->SetMessageEncoder(mStreetGangMessageRequestEncoder); \
            return reactor; \
        } \
    } \
    break

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


StreetGangClientMessageDecoder::StreetGangClientMessageDecoder()
{
    mStreetGangMessageRequestEncoder = std::make_shared<StreetGangMessageRequestEncoder>();
}

StreetGangClientMessageDecoder::~StreetGangClientMessageDecoder()
{
}

std::shared_ptr<Reactor> StreetGangClientMessageDecoder::Decode(std::istream& stream, Connection& connection)
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
        CREATE_RESPONSE_REACTOR(streetgangapi::ID::Error, ResponseError, ResponseErrorReactor);
        CREATE_RESPONSE_REACTOR(streetgangapi::ID::GetVersionResponse, ResponseGetVersion, ResponseGetVersionReactor);
        CREATE_RESPONSE_REACTOR(streetgangapi::ID::CreateWorldResponse, ResponseCreateWorld, ResponseCreateWorldReactor);
        CREATE_RESPONSE_REACTOR(streetgangapi::ID::GetSceneResponse, ResponseGetScene, ResponseGetSceneReactor);

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

    LOG("[" FMT_INT64 "] StreetGangClientMessageDecoder::Decode() [ID=%d] [Error=Failed to decode message]",
        std::chrono::high_resolution_clock::now().time_since_epoch().count(),
        id);

    return nullptr;
}

bool StreetGangClientMessageDecoder::DecodeMessage(std::istream& stream, sg::microreactor::Message& message)
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

bool StreetGangClientMessageDecoder::DecodeMessage(std::istream& stream, MessageBase& message)
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

bool StreetGangClientMessageDecoder::DecodeMessage(std::istream& stream, ResponseError& message)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    if (!DecodeMessage(stream, static_cast<MessageBase&>(message)))
    {
        return false;
    }

    int32_t requestId = 0;
    if (serializer.Read(stream, requestId))
    {
        message.RequestId.set(requestId);
    }
    else
    {
        return false;
    }

    std::string errorMessage;
    if (serializer.Read(stream, errorMessage))
    {
        message.ErrorMessage.set(errorMessage);
    }
    else
    {
        return false;
    }

    return true;
}

bool StreetGangClientMessageDecoder::DecodeMessage(std::istream& stream, ResponseGetVersion& message)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    if (!DecodeMessage(stream, static_cast<MessageBase&>(message)))
    {
        return false;
    }

    std::string version;
    if (serializer.Read(stream, version))
    {
        message.Version.set(version);
    }
    else
    {
        return false;
    }

    return true;
}

bool StreetGangClientMessageDecoder::DecodeMessage(std::istream& stream, ResponseCreateWorld& message)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    if (!DecodeMessage(stream, static_cast<MessageBase&>(message)))
    {
        return false;
    }

    SessionId worldId = InvalidSessionId;
    if (serializer.Read(stream, worldId))
    {
        message.WorldId.set(worldId);
    }
    else
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

bool StreetGangClientMessageDecoder::DecodeMessage(std::istream& stream, ResponseGetScene& message)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    if (!DecodeMessage(stream, static_cast<MessageBase&>(message)))
    {
        LOG("[" FMT_INT64 "] StreetGangClientMessageDecoder::DecodeMessage() [Error=Failed to decode MessageBase]",
            std::chrono::high_resolution_clock::now().time_since_epoch().count());

        return false;
    }

    auto worldId = DefaultSessionId;
    if (serializer.Read(stream, worldId))
    {
        message.WorldId.set(worldId);
    }
    else
    {
        LOG("[" FMT_INT64 "] StreetGangClientMessageDecoder::DecodeMessage() [Error=Failed to decode worldId]",
            std::chrono::high_resolution_clock::now().time_since_epoch().count());

        return false;
    }

    auto rect = message.Rect.cref();
    if (serializer.Read(stream, rect))
    {
        message.Rect.set(rect);
    }
    else
    {
        LOG("[" FMT_INT64 "] StreetGangClientMessageDecoder::DecodeMessage() [Error=Failed to decode rect]",
            std::chrono::high_resolution_clock::now().time_since_epoch().count());

        return false;
    }

    std::vector<Point<float>> items;
    if (SerializerRead(serializer, stream, items))
    {
        message.Items.set(items);
    }
    else
    {
        LOG("[" FMT_INT64 "] StreetGangClientMessageDecoder::DecodeMessage() [Error=Failed to decode items]",
            std::chrono::high_resolution_clock::now().time_since_epoch().count());
        
        return false;
    }

    return true;
}
