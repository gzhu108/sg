#include "BinaryRequestGetScene.h"
#include "BinarySerializer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


BinaryRequestGetScene::BinaryRequestGetScene()
{
}

BinaryRequestGetScene::~BinaryRequestGetScene()
{
}

bool BinaryRequestGetScene::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(MessageSessionId.cref(), stream)
        && serializer.Write(WorldId.cref(), stream)
        && serializer.Write(Rect.cref(), stream);
}

bool BinaryRequestGetScene::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, MessageSessionId);
    serializer.ReadProperty(stream, WorldId);
    serializer.ReadProperty(stream, Rect);

    return true;
}
