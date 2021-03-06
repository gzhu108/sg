#include "BinaryResponseCreateWorld.h"
#include "BinarySerializer.h"

using namespace microreactor;
using namespace streetgangapi;


BinaryResponseCreateWorld::BinaryResponseCreateWorld()
{
}

BinaryResponseCreateWorld::~BinaryResponseCreateWorld()
{
}

bool BinaryResponseCreateWorld::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(MessageSessionId.cref(), stream)
        && serializer.Write(WorldId.cref(), stream)
        && serializer.Write(WorldName.cref(), stream);
}

bool BinaryResponseCreateWorld::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, MessageSessionId);
    serializer.ReadProperty(stream, WorldId);
    serializer.ReadProperty(stream, WorldName);

    return true;
}
