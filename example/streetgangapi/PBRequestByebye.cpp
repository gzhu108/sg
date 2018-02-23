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
    streetgangpb::ByebyeRequest request;
    request.set_track_id(TrackId.cref());

    return Serialize("ByebyeRequest", request, stream);
}

bool PBRequestByebye::Decode(std::istream& stream)
{
    streetgangpb::ByebyeRequest request;
    if (!Deserialize(stream, request))
    {
        return false;
    }

    TrackId.set(request.track_id());
    
    return true;
}
