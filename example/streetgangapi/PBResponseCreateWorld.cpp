#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
#include "protobuf/cpp/CreateWorldResponse.pb.h"
#include "PBResponseCreateWorld.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBResponseCreateWorld::PBResponseCreateWorld()
{
}

PBResponseCreateWorld::~PBResponseCreateWorld()
{
}

bool PBResponseCreateWorld::Encode(std::ostream& stream) const
{
    streetgangpb::MessageHeader header;
    header.set_message_id("CreateWorldResponse");

    streetgangpb::CreateWorldResponse response;
    response.set_track_id(TrackId.cref());
    response.set_result(Result.cref());
    response.set_world_id(WorldId.cref());
    response.set_world_name(WorldName.cref());

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)header.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    header.SerializeToCodedStream(&codedOutputStream);

    size = (uint32_t)response.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return response.SerializeToCodedStream(&codedOutputStream);
}

bool PBResponseCreateWorld::Decode(std::istream& stream)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::CreateWorldResponse response;
    bool result = response.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previousSize);
    if (!result)
    {
        return nullptr;
    }

    TrackId.set(response.track_id());
    Result.set(response.result());
    WorldId.set(response.world_id());
    WorldName.set(response.world_name());

    return true;
}
