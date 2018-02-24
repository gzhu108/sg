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

bool PBRequestByebye::Decode(google::protobuf::io::CodedInputStream& codedInputStream)
{
    streetgangpb::ByebyeRequest request;
    if (!Deserialize(codedInputStream, request))
    {
        return false;
    }

    TrackId.set(request.track_id());
    
    return true;
}
