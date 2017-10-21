#include "StreetGangMessageResponseEncoder.h"
#include "Serializer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


StreetGangMessageResponseEncoder::StreetGangMessageResponseEncoder()
{
}

StreetGangMessageResponseEncoder::~StreetGangMessageResponseEncoder()
{
}

bool StreetGangMessageResponseEncoder::EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);
    std::stringstream messageStream;

    return EncodeMessage(static_cast<MessageBase&>(*message), messageStream)
        && serializer.Write(message->RequestId.cref(), messageStream)
        && serializer.Write(message->ErrorMessage.cref(), messageStream)
        && EncodeMessage(std::static_pointer_cast<MessageBase>(message), messageStream, stream);
}

bool StreetGangMessageResponseEncoder::EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);
    std::stringstream messageStream;

    return EncodeMessage(static_cast<MessageBase&>(*message), messageStream)
        && serializer.Write(message->Version.cref(), messageStream)
        && EncodeMessage(std::static_pointer_cast<MessageBase>(message), messageStream, stream);
}

bool StreetGangMessageResponseEncoder::EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);
    std::stringstream messageStream;

    return EncodeMessage(static_cast<MessageBase&>(*message), messageStream)
        && serializer.Write(message->WorldId.cref(), messageStream)
        && serializer.Write(message->WorldName.cref(), messageStream)
        && EncodeMessage(std::static_pointer_cast<MessageBase>(message), messageStream, stream);
}

bool StreetGangMessageResponseEncoder::EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);
    std::stringstream messageStream;

    return EncodeMessage(static_cast<MessageBase&>(*message), messageStream)
        && serializer.Write(message->WorldId.cref(), messageStream)
        && serializer.Write(message->Rect.cref(), messageStream)
        && SerializerWrite(serializer, message->Items.cref(), messageStream)
        && EncodeMessage(std::static_pointer_cast<MessageBase>(message), messageStream, stream);
}

bool StreetGangMessageResponseEncoder::EncodeMessage(std::shared_ptr<MessageBase> message, std::istream& inStream, std::ostream& outStream)
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

bool StreetGangMessageResponseEncoder::EncodeMessage(sg::microreactor::Message& message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    return serializer.Write(message.TrackId.cref(), stream)
        && serializer.Write(message.ResponseTimeout.cref(), stream);
}

bool StreetGangMessageResponseEncoder::EncodeMessage(MessageBase& message, std::ostream& stream)
{
    Serializer& serializer = static_cast<Serializer&>(mSerializer);

    return EncodeMessage(static_cast<Message&>(message), stream)
        && serializer.Write(message.Result.cref(), stream)
        && serializer.Write(message.MessageSessionId.cref(), stream);
}
