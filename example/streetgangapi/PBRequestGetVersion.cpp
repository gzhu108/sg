#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
#include "protobuf/cpp/GetVersionRequest.pb.h"
#include "PBRequestGetVersion.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBRequestGetVersion::PBRequestGetVersion()
{
}

PBRequestGetVersion::~PBRequestGetVersion()
{
}

bool PBRequestGetVersion::Encode(std::ostream& stream) const
{
    auto trackId = TrackId.cref();

    streetgangpb::MessageHeader header;
    header.set_message_id("ByebyeRequest");

    streetgangpb::GetVersionRequest request;
    request.set_track_id(TrackId.cref());

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)header.ByteSize();
    codedOutputStream.WriteVarint32(size);
    header.SerializeWithCachedSizes(&codedOutputStream);

    size = (uint32_t)request.ByteSize();
    codedOutputStream.WriteVarint32(size);
    request.SerializeWithCachedSizes(&codedOutputStream);

    return true;
}

bool PBRequestGetVersion::Decode(std::istream& stream)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::GetVersionRequest request;
    bool result = request.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previousSize);
    if (!result)
    {
        return false;
    }

    TrackId.set(request.track_id());

    return true;
}
