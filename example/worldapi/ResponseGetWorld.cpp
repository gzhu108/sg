#include "ResponseGetWorld.h"
#include "BinarySerializer.h"

using namespace sg::microreactor;
using namespace worldapi;


ResponseGetWorld::ResponseGetWorld()
{
    Id.set((int32_t)ID::GetWorldResponse);
}

ResponseGetWorld::~ResponseGetWorld()
{
}

bool ResponseGetWorld::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(World.cref(), stream);
}

bool ResponseGetWorld::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, World);

    return true;
}
