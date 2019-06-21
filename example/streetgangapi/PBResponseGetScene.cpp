#include "GetSceneResponse.pb.h"
#include "PBResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBResponseGetScene::PBResponseGetScene()
{
}

PBResponseGetScene::~PBResponseGetScene()
{
}

bool PBResponseGetScene::Encode(std::ostream& stream) const
{
    streetgangpb::GetSceneResponse response;
    response.set_track_id(TrackId.cref());
    response.set_result(Result.cref());
    response.set_world_id(WorldId.cref());
    response.mutable_rect()->set_x(Rect->mX);
    response.mutable_rect()->set_y(Rect->mY);
    response.mutable_rect()->set_w(Rect->mW);
    response.mutable_rect()->set_h(Rect->mH);

    return Serialize("GetSceneResponse", response, stream);
}

bool PBResponseGetScene::Decode(google::protobuf::io::CodedInputStream& codedInputStream)
{
    streetgangpb::GetSceneResponse response;
    if (!Deserialize(codedInputStream, response))
    {
        return false;
    }

    TrackId.set(response.track_id());
    Result.set(response.result());
    WorldId.set(response.world_id());
    Rect->mX = response.rect().x();
    Rect->mY = response.rect().y();
    Rect->mW = response.rect().w();
    Rect->mH = response.rect().h();

    return true;
}
