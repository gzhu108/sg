#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
#include "protobuf/cpp/ByebyeRequest.pb.h"
#include "PBRequestByebye.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBRequestByebye::PBRequestByebye()
{
}

PBRequestByebye::~PBRequestByebye()
{
}

bool PBRequestByebye::Encode(std::ostream& stream) const
{
    auto trackId = TrackId.cref();

    streetgangpb::MessageHeader header;
    header.set_message_id("ByebyeRequest");

    streetgangpb::ByebyeRequest request;
    request.set_track_id(TrackId.cref());

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)header.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    header.SerializeToCodedStream(&codedOutputStream);

    size = (uint32_t)request.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return request.SerializeToCodedStream(&codedOutputStream);
}

bool PBRequestByebye::Decode(std::istream& stream)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::ByebyeRequest request;
    bool result = request.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previousSize);
    if (!result)
    {
        return false;
    }

    TrackId.set(request.track_id());

    return true;
}