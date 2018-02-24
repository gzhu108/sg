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
    streetgangpb::CreateWorldResponse response;
    response.set_track_id(TrackId.cref());
    response.set_result(Result.cref());
    response.set_world_id(WorldId.cref());
    response.set_world_name(WorldName.cref());

    return Serialize("CreateWorldResponse", response, stream);
}

bool PBResponseCreateWorld::Decode(google::protobuf::io::CodedInputStream& codedInputStream)
{
    streetgangpb::CreateWorldResponse response;
    if (!Deserialize(codedInputStream, response))
    {
        return false;
    }

    TrackId.set(response.track_id());
    Result.set(response.result());
    WorldId.set(response.world_id());
    WorldName.set(response.world_name());

    return true;
}
