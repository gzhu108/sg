#include "BinaryRequestCreateWorld.h"
#include "BinarySerializer.h"

#define READ_PROPERTY(_property) \
    PROPERTY_TYPE(_property) _property##Value; \
    if (serializer.Read(stream, _property##Value)) \
        _property.set(_property##Value); \
    else \
        return false

using namespace sg::microreactor;
using namespace streetgangapi;


BinaryRequestCreateWorld::BinaryRequestCreateWorld()
{
}

BinaryRequestCreateWorld::~BinaryRequestCreateWorld()
{
}

bool BinaryRequestCreateWorld::Serialize(std::ostream& stream) const
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

bool BinaryRequestCreateWorld::Deserialize(std::istream& stream)
{
    BinarySerializer serializer;

    READ_PROPERTY(TrackId);
    READ_PROPERTY(ResponseTimeout);
    READ_PROPERTY(Result);
    READ_PROPERTY(MessageSessionId);
    READ_PROPERTY(WorldName);

    return true;
}
