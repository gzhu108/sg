#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
#include "protobuf/cpp/ErrorResponse.pb.h"
#include "PBResponseError.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBResponseError::PBResponseError()
{
}

PBResponseError::~PBResponseError()
{
}

bool PBResponseError::Encode(std::ostream& stream) const
{
    streetgangpb::MessageHeader header;
    header.set_message_id("ErrorResponse");

    streetgangpb::ErrorResponse response;
    response.set_track_id(TrackId.cref());
    response.set_result(Result.cref());
    response.set_request_id(RequestId.cref());
    response.set_error_message(ErrorMessage.cref());

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)header.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    header.SerializeToCodedStream(&codedOutputStream);

    size = (uint32_t)response.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return response.SerializeToCodedStream(&codedOutputStream);
}

bool PBResponseError::Decode(std::istream& stream)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::ErrorResponse response;
    bool result = response.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previousSize);
    if (!result)
    {
        return nullptr;
    }

    TrackId.set(response.track_id());
    Result.set(response.result());
    RequestId.set(response.request_id());
    ErrorMessage.set(response.error_message());

    return true;
}
