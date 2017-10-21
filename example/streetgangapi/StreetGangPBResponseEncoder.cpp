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
#include "StreetGangPBResponseEncoder.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangpb;


StreetGangPBResponseEncoder::StreetGangPBResponseEncoder()
{
}

StreetGangPBResponseEncoder::~StreetGangPBResponseEncoder()
{
}

bool StreetGangPBResponseEncoder::EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    auto trackId = message->TrackId.cref();
    auto& result = message->Result.cref();
    auto& requestId = message->RequestId.cref();
    auto& errorMessage = message->ErrorMessage.cref();

    streetgangpb::MessageHeader pbMessageHeader;
    pbMessageHeader.set_track_id(trackId);
    pbMessageHeader.set_message_id("ErrorResponse");
    pbMessageHeader.set_result(result);

    ErrorResponse pbErrorResponse;
    pbErrorResponse.set_request_id(requestId);
    pbErrorResponse.set_error_message(errorMessage);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)pbMessageHeader.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    pbMessageHeader.SerializeToCodedStream(&codedOutputStream);

    size = (int32_t)pbErrorResponse.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return pbErrorResponse.SerializeToCodedStream(&codedOutputStream);
}

bool StreetGangPBResponseEncoder::EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    auto trackId = message->TrackId.cref();
    auto& result = message->Result.cref();
    auto& version = message->Version.cref();

    streetgangpb::MessageHeader pbMessageHeader;
    pbMessageHeader.set_track_id(trackId);
    pbMessageHeader.set_message_id("GetVersionResponse");
    pbMessageHeader.set_result(result);

    GetVersionResponse pbGetVersionResponse;
    pbGetVersionResponse.set_version(version);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)pbMessageHeader.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    pbMessageHeader.SerializeToCodedStream(&codedOutputStream);

    size = (int32_t)pbGetVersionResponse.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return pbGetVersionResponse.SerializeToCodedStream(&codedOutputStream);
}

bool StreetGangPBResponseEncoder::EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    auto trackId = message->TrackId.cref();
    auto& result = message->Result.cref();
    auto& worldId = message->WorldId.cref();
    auto& worldName = message->WorldName.cref();

    streetgangpb::MessageHeader pbMessageHeader;
    pbMessageHeader.set_track_id(trackId);
    pbMessageHeader.set_message_id("CreateWorldResponse");
    pbMessageHeader.set_result(result);

    CreateWorldResponse pbCreateWorldResponse;
    pbCreateWorldResponse.set_world_id(worldId);
    pbCreateWorldResponse.set_world_name(worldName);

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)pbMessageHeader.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    pbMessageHeader.SerializeToCodedStream(&codedOutputStream);

    size = (int32_t)pbCreateWorldResponse.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return pbCreateWorldResponse.SerializeToCodedStream(&codedOutputStream);
}

bool StreetGangPBResponseEncoder::EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    auto trackId = message->TrackId.cref();
    auto& result = message->Result.cref();
    auto& worldId = message->WorldId.cref();
    auto& rect = message->Rect.cref();
    auto& items = message->Items.cref();

    streetgangpb::MessageHeader pbMessageHeader;
    pbMessageHeader.set_track_id(trackId);
    pbMessageHeader.set_message_id("GetSceneResponse");
    pbMessageHeader.set_result(result);

    GetSceneResponse pbGetSceneResponse;
    pbGetSceneResponse.set_world_id(worldId);
    pbGetSceneResponse.mutable_rect()->set_x(rect.mX);
    pbGetSceneResponse.mutable_rect()->set_y(rect.mY);
    pbGetSceneResponse.mutable_rect()->set_w(rect.mW);
    pbGetSceneResponse.mutable_rect()->set_h(rect.mH);

    auto pbItems = pbGetSceneResponse.mutable_items();
    for (auto& item : items)
    {
        auto pbItem = pbItems->Add();
        pbItem->set_x(item.mX);
        pbItem->set_y(item.mY);
        pbItem->set_z(item.mZ);
    }

    google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
    google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

    uint32_t size = (uint32_t)pbMessageHeader.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    pbMessageHeader.SerializeToCodedStream(&codedOutputStream);

    size = (int32_t)pbGetSceneResponse.ByteSizeLong();
    codedOutputStream.WriteVarint32(size);
    return pbGetSceneResponse.SerializeToCodedStream(&codedOutputStream);
}
