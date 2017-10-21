#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
#include "protobuf/cpp/ErrorResponse.pb.h"
#include "protobuf/cpp/CreateWorldRequest.pb.h"
#include "protobuf/cpp/CreateWorldResponse.pb.h"
#include "protobuf/cpp/GetSceneRequest.pb.h"
#include "protobuf/cpp/GetSceneResponse.pb.h"
#include "protobuf/cpp/GetVersionResponse.pb.h"
#include "StreetGangPBRequestEncoder.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangpb;


StreetGangPBRequestEncoder::StreetGangPBRequestEncoder()
{
}

StreetGangPBRequestEncoder::~StreetGangPBRequestEncoder()
{
}

bool StreetGangPBRequestEncoder::EncodeMessage(std::shared_ptr<RequestByebye> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    auto trackId = message->TrackId.cref();

    streetgangpb::MessageHeader pbMessageHeader;
    pbMessageHeader.set_track_id(trackId);
    pbMessageHeader.set_message_id("ByebyeRequest");
    pbMessageHeader.set_result(0);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)pbMessageHeader.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return pbMessageHeader.SerializeToCodedStream(&codedOutputStream);
}

bool StreetGangPBRequestEncoder::EncodeMessage(std::shared_ptr<RequestGetVersion> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    auto trackId = message->TrackId.cref();

    streetgangpb::MessageHeader pbMessageHeader;
    pbMessageHeader.set_track_id(trackId);
    pbMessageHeader.set_message_id("GetVersionRequest");
    pbMessageHeader.set_result(0);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)pbMessageHeader.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return pbMessageHeader.SerializeToCodedStream(&codedOutputStream);
}

bool StreetGangPBRequestEncoder::EncodeMessage(std::shared_ptr<RequestCreateWorld> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    auto trackId = message->TrackId.cref();
    auto& worldName = message->WorldName.cref();

    streetgangpb::MessageHeader pbMessageHeader;
    pbMessageHeader.set_track_id(trackId);
    pbMessageHeader.set_message_id("CreateWorldRequest");
    pbMessageHeader.set_result(0);

    CreateWorldRequest pbCreateWorldRequest;
    pbCreateWorldRequest.set_world_name(worldName);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)pbMessageHeader.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    pbMessageHeader.SerializeToCodedStream(&codedOutputStream);

    size = (int32_t)pbCreateWorldRequest.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return pbCreateWorldRequest.SerializeToCodedStream(&codedOutputStream);
}

bool StreetGangPBRequestEncoder::EncodeMessage(std::shared_ptr<RequestGetScene> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    auto trackId = message->TrackId.cref();
    auto& worldId = message->WorldId.cref();
    auto& rect = message->Rect.cref();

    streetgangpb::MessageHeader pbMessageHeader;
    pbMessageHeader.set_track_id(trackId);
    pbMessageHeader.set_message_id("GetSceneRequest");
    pbMessageHeader.set_result(0);

    GetSceneRequest pbGetSceneRequest;
    pbGetSceneRequest.set_world_id(worldId);
    pbGetSceneRequest.mutable_rect()->set_x(rect.mX);
    pbGetSceneRequest.mutable_rect()->set_y(rect.mY);
    pbGetSceneRequest.mutable_rect()->set_w(rect.mW);
    pbGetSceneRequest.mutable_rect()->set_h(rect.mH);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)pbMessageHeader.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    pbMessageHeader.SerializeToCodedStream(&codedOutputStream);

    size = (int32_t)pbGetSceneRequest.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return pbGetSceneRequest.SerializeToCodedStream(&codedOutputStream);
}
