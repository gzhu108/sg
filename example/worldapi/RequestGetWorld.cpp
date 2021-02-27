#include "RequestGetWorld.h"
#include "BinarySerializer.h"

using namespace microreactor;
using namespace worldapi;


RequestGetWorld::RequestGetWorld()
{
    Id.set((int)ID::GetWorldRequest);
}

RequestGetWorld::~RequestGetWorld()
{
}

bool RequestGetWorld::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(WorldId.cref(), stream);
}

bool RequestGetWorld::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, WorldId);

    return true;
}
