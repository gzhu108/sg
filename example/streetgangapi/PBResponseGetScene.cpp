#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
#include "protobuf/cpp/GetSceneResponse.pb.h"
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
    streetgangpb::MessageHeader header;
    header.set_message_id("GetSceneResponse");

    streetgangpb::GetSceneResponse response;
    response.set_track_id(TrackId.cref());
    response.set_result(Result.cref());
    response.set_world_id(WorldId.cref());
    response.mutable_rect()->set_x(Rect->mX);
    response.mutable_rect()->set_y(Rect->mY);
    response.mutable_rect()->set_w(Rect->mW);
    response.mutable_rect()->set_h(Rect->mH);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)header.ByteSize();
    codedOutputStream.WriteVarint32(size);
    header.SerializeWithCachedSizes(&codedOutputStream);

    size = (uint32_t)response.ByteSize();
    codedOutputStream.WriteVarint32(size);
    response.SerializeWithCachedSizes(&codedOutputStream);

    return true;
}

bool PBResponseGetScene::Decode(std::istream& stream)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::GetSceneResponse response;
    bool result = response.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previousSize);
    if (!result)
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
