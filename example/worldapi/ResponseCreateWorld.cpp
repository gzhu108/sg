#include "ResponseCreateWorld.h"
#include "BinarySerializer.h"

using namespace microreactor;
using namespace worldapi;


ResponseCreateWorld::ResponseCreateWorld()
{
    Id.set((int32_t)ID::CreateWorldResponse);
}

ResponseCreateWorld::~ResponseCreateWorld()
{
}

bool ResponseCreateWorld::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(WorldId.cref(), stream)
        && serializer.Write(WorldName.cref(), stream);
}

bool ResponseCreateWorld::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, WorldId);
    serializer.ReadProperty(stream, WorldName);

    return true;
}
