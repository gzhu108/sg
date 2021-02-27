#include "ErrorResponse.pb.h"
#include "PBResponseError.h"

using namespace microreactor;
using namespace streetgangapi;


PBResponseError::PBResponseError()
{
}

PBResponseError::~PBResponseError()
{
}

bool PBResponseError::Encode(std::ostream& stream) const
{
    streetgangpb::ErrorResponse response;
    response.set_track_id(TrackId.cref());
    response.set_result(Result.cref());
    response.set_request_id(RequestId.cref());
    response.set_error_message(ErrorMessage.cref());

    return Serialize("ErrorResponse", response, stream);
}

bool PBResponseError::Decode(google::protobuf::io::CodedInputStream& codedInputStream)
{
    streetgangpb::ErrorResponse response;
    if (!Deserialize(codedInputStream, response))
    {
        return false;
    }

    TrackId.set(response.track_id());
    Result.set(response.result());
    RequestId.set(response.request_id());
    ErrorMessage.set(response.error_message());

    return true;
}
