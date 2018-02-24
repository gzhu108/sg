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
    streetgangpb::GetVersionResponse response;
    response.set_track_id(TrackId.cref());
    response.set_result(Result.cref());
    response.set_version(Version.cref());

    return Serialize("GetVersionResponse", response, stream);
}

bool PBResponseGetVersion::Decode(google::protobuf::io::CodedInputStream& codedInputStream)
{
    streetgangpb::GetVersionResponse response;
    if (!Deserialize(codedInputStream, response))
    {
        return false;
    }

    TrackId.set(response.track_id());
    Result.set(response.result());
    Version.set(response.version());

    return true;
}
