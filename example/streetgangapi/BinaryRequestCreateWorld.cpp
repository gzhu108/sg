#include "BinaryRequestCreateWorld.h"
#include "BinarySerializer.h"

using namespace microreactor;
using namespace streetgangapi;


BinaryRequestCreateWorld::BinaryRequestCreateWorld()
{
}

BinaryRequestCreateWorld::~BinaryRequestCreateWorld()
{
}

bool BinaryRequestCreateWorld::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(MessageSessionId.cref(), stream)
        && serializer.Write(WorldName.cref(), stream);
}

bool BinaryRequestCreateWorld::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, MessageSessionId);
    serializer.ReadProperty(stream, WorldName);

    return true;
}
