#include "protobuf/cpp/GetSceneRequest.pb.h"
#include "PBRequestGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBRequestGetScene::PBRequestGetScene()
{
}

PBRequestGetScene::~PBRequestGetScene()
{
}

bool PBRequestGetScene::Encode(std::ostream& stream) const
{
    streetgangpb::GetSceneRequest request;
    request.set_track_id(TrackId.cref());
    request.set_world_id(WorldId.cref());
    request.mutable_rect()->set_x(Rect->mX);
    request.mutable_rect()->set_y(Rect->mY);
    request.mutable_rect()->set_w(Rect->mW);
    request.mutable_rect()->set_h(Rect->mH);

    return Serialize("GetSceneRequest", request, stream);
}

bool PBRequestGetScene::Decode(std::istream& stream)
{
    streetgangpb::GetSceneRequest request;
    if (!Deserialize(stream, request))
    {
        return false;
    }

    TrackId.set(request.track_id());
    WorldId.set(request.world_id());
    Rect->mX = request.rect().x();
    Rect->mY = request.rect().y();
    Rect->mW = request.rect().w();
    Rect->mH = request.rect().h();

    return true;
}
