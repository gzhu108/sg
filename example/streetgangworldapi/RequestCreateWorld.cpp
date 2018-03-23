#include "RequestCreateWorld.h"
#include "BinarySerializer.h"

using namespace sg::microreactor;
using namespace streetgangworldapi;


RequestCreateWorld::RequestCreateWorld()
{
    Id.set((int)ID::CreateWorldRequest);
}

RequestCreateWorld::~RequestCreateWorld()
{
}

bool RequestCreateWorld::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(WorldName.cref(), stream);
}

bool RequestCreateWorld::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, WorldName);

    return true;
}
