#include "BinaryResponseGetVersion.h"
#include "BinarySerializer.h"

using namespace microreactor;
using namespace streetgangapi;


BinaryResponseGetVersion::BinaryResponseGetVersion()
{
}

BinaryResponseGetVersion::~BinaryResponseGetVersion()
{
}

bool BinaryResponseGetVersion::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(MessageSessionId.cref(), stream)
        && serializer.Write(Version.cref(), stream);
}

bool BinaryResponseGetVersion::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, MessageSessionId);
    serializer.ReadProperty(stream, Version);

    return true;
}
