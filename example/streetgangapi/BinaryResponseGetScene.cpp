#include "BinaryResponseGetScene.h"
#include "BinarySerializer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


BinaryResponseGetScene::BinaryResponseGetScene()
{
}

BinaryResponseGetScene::~BinaryResponseGetScene()
{
}

bool BinaryResponseGetScene::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(MessageSessionId.cref(), stream)
        && serializer.Write(WorldId.cref(), stream)
        && serializer.Write(Rect.cref(), stream)
        && SerializerWrite(serializer, Items.cref(), stream);
}

bool BinaryResponseGetScene::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, MessageSessionId);
    serializer.ReadProperty(stream, WorldId);
    serializer.ReadProperty(stream, Rect);
    SerializerRead(serializer, stream, Items.ref());

    return true;
}
