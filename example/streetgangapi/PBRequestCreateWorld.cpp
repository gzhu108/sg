#include "CreateWorldRequest.pb.h"
#include "PBRequestCreateWorld.h"

using namespace microreactor;
using namespace streetgangapi;


PBRequestCreateWorld::PBRequestCreateWorld()
{
}

PBRequestCreateWorld::~PBRequestCreateWorld()
{
}

bool PBRequestCreateWorld::Encode(std::ostream& stream) const
{
    streetgangpb::CreateWorldRequest request;
    request.set_track_id(TrackId.cref());
    request.set_world_name(WorldName.cref());

    return Serialize("CreateWorldRequest", request, stream);
}

bool PBRequestCreateWorld::Decode(google::protobuf::io::CodedInputStream& codedInputStream)
{
    streetgangpb::CreateWorldRequest request;
    if (!Deserialize(codedInputStream, request))
    {
        return false;
    }

    TrackId.set(request.track_id());
    WorldName.set(request.world_name());

    return true;
}
