#include "StreetGangMessageRequestEncoder.h"
#include "Serializer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


StreetGangMessageRequestEncoder::StreetGangMessageRequestEncoder()
{
}

StreetGangMessageRequestEncoder::~StreetGangMessageRequestEncoder()
{
}

bool StreetGangMessageRequestEncoder::EncodeMessage(std::shared_ptr<RequestByebye> message, std::ostream& stream)
{
    std::stringstream messageStream;

    return EncodeMessage(static_cast<MessageBase&>(*message), messageStream)
        && EncodeMessage(std::static_pointer_cast<MessageBase>(message), messageStream, stream);
}

bool StreetGangMessageRequestEncoder::EncodeMessage(std::shared_ptr<RequestGetVersion> message, std::ostream& stream)
{
    std::stringstream messageStream;

    return EncodeMessage(static_cast<MessageBase&>(*message), messageStream)
        && EncodeMessage(std::static_pointer_cast<MessageBase>(message), messageStream, stream);
}

bool StreetGangMessageRequestEncoder::EncodeMessage(std::shared_ptr<RequestCreateWorld> message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);
    std::stringstream messageStream;

    return EncodeMessage(static_cast<MessageBase&>(*message), messageStream)
        && serializer.Write(message->WorldName.cref(), messageStream)
        && EncodeMessage(std::static_pointer_cast<MessageBase>(message), messageStream, stream);
}

bool StreetGangMessageRequestEncoder::EncodeMessage(std::shared_ptr<RequestGetScene> message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);
    std::stringstream messageStream;

    return EncodeMessage(static_cast<MessageBase&>(*message), messageStream)
        && serializer.Write(message->WorldId.cref(), messageStream)
        && serializer.Write(message->Rect.cref(), messageStream)
        && EncodeMessage(std::static_pointer_cast<MessageBase>(message), messageStream, stream);
}

bool StreetGangMessageRequestEncoder::EncodeMessage(std::shared_ptr<MessageBase> message, std::istream& inStream, std::ostream& outStream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    //if (!serializer.Write(MARKER, outStream))
    //{
    //    return false;
    //}

    if (!serializer.Write(message->Id.cref(), outStream))
    {
        return false;
    }

    uint64_t length = GetStreamSize(inStream);
    if (!serializer.Write(length, outStream))
    {
        return false;
    }

    if (!serializer.Write(inStream, outStream))
    {
        return false;
    }

    return true;
}

bool StreetGangMessageRequestEncoder::EncodeMessage(sg::microreactor::Message& message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    return serializer.Write(message.TrackId.cref(), stream)
        && serializer.Write(message.ResponseTimeout.cref(), stream);
}

bool StreetGangMessageRequestEncoder::EncodeMessage(MessageBase& message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    return EncodeMessage(static_cast<Message&>(message), stream)
        && serializer.Write(message.Result.cref(), stream)
        && serializer.Write(message.MessageSessionId.cref(), stream);
}
