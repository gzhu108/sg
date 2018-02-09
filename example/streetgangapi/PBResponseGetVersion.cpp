#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
#include "protobuf/cpp/GetVersionResponse.pb.h"
#include "PBResponseGetVersion.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBResponseGetVersion::PBResponseGetVersion()
{
}

PBResponseGetVersion::~PBResponseGetVersion()
{
}

bool PBResponseGetVersion::Encode(std::ostream& stream) const
{
    streetgangpb::MessageHeader header;
    header.set_message_id("GetVersionResponse");

    streetgangpb::GetVersionResponse response;
    response.set_track_id(TrackId.cref());
    response.set_result(Result.cref());
    response.set_version(Version.cref());

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)header.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    header.SerializeToCodedStream(&codedOutputStream);

    size = (uint32_t)response.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return response.SerializeToCodedStream(&codedOutputStream);
}

bool PBResponseGetVersion::Decode(std::istream& stream)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::GetVersionResponse response;
    bool result = response.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previousSize);
    if (!result)
    {
        return false;
    }

    TrackId.set(response.track_id());
    Result.set(response.result());
    Version.set(response.version());

    return true;
}
