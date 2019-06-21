#include "GetVersionRequest.pb.h"
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
    streetgangpb::GetVersionRequest request;
    request.set_track_id(TrackId.cref());

    return Serialize("GetVersionRequest", request, stream);
}

bool PBRequestGetVersion::Decode(google::protobuf::io::CodedInputStream& codedInputStream)
{
    streetgangpb::GetVersionRequest request;
    if (!Deserialize(codedInputStream, request))
    {
        return false;
    }

    TrackId.set(request.track_id());

    return true;
}
