#include "BinaryResponseError.h"
#include "BinarySerializer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


BinaryResponseError::BinaryResponseError()
{
}

BinaryResponseError::~BinaryResponseError()
{
}

bool BinaryResponseError::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(MessageSessionId.cref(), stream)
        && serializer.Write(RequestId.cref(), stream)
        && serializer.Write(ErrorMessage.cref(), stream);
}

bool BinaryResponseError::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, MessageSessionId);
    serializer.ReadProperty(stream, RequestId);
    serializer.ReadProperty(stream, ErrorMessage);

    return true;
}
