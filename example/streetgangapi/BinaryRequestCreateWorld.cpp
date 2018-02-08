#include "BinaryRequestCreateWorld.h"
#include "BinarySerializer.h"

using namespace sg::microreactor;
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

    /*
    if (!result)
    {
        return false;
    }

    uint64_t length = GetStreamSize(messageStream);

    return serializer.Write(Id.cref(), stream)
        && serializer.Write(length, stream)
        && serializer.Write(messageStream, stream);
    */
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
