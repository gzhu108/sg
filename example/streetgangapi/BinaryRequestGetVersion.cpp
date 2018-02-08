#include "BinaryRequestGetVersion.h"
#include "BinarySerializer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


BinaryRequestGetVersion::BinaryRequestGetVersion()
{
}

BinaryRequestGetVersion::~BinaryRequestGetVersion()
{
}

bool BinaryRequestGetVersion::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(MessageSessionId.cref(), stream);
}

bool BinaryRequestGetVersion::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, MessageSessionId);

    return true;
}
