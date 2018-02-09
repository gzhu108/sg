#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
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
    auto trackId = TrackId.cref();

    streetgangpb::MessageHeader header;
    header.set_message_id("ByebyeRequest");

    streetgangpb::GetSceneRequest request;
    request.set_track_id(TrackId.cref());
    request.set_world_id(WorldId.cref());
    request.mutable_rect()->set_x(Rect->mX);
    request.mutable_rect()->set_y(Rect->mY);
    request.mutable_rect()->set_w(Rect->mW);
    request.mutable_rect()->set_h(Rect->mH);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)header.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    header.SerializeToCodedStream(&codedOutputStream);

    size = (uint32_t)request.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return request.SerializeToCodedStream(&codedOutputStream);
}

bool PBRequestGetScene::Decode(std::istream& stream)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::GetSceneRequest request;
    bool result = request.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previousSize);
    if (!result)
    {
        return nullptr;
    }

    TrackId.set(request.track_id());
    WorldId.set(request.world_id());
    Rect->mX = request.rect().x();
    Rect->mY = request.rect().y();
    Rect->mW = request.rect().w();
    Rect->mH = request.rect().h();

    return true;
}
