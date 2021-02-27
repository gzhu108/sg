#include "BinaryRequestByebye.h"
#include "BinarySerializer.h"

using namespace microreactor;
using namespace streetgangapi;


BinaryRequestByebye::BinaryRequestByebye()
{
}

BinaryRequestByebye::~BinaryRequestByebye()
{
}

bool BinaryRequestByebye::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(MessageSessionId.cref(), stream);
}

bool BinaryRequestByebye::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, MessageSessionId);

    return true;
}
