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

#include "StreetGangClientPBDecoder.h"
#include "StreetGangIds.h"

#include "ResponseErrorReactor.h"
#include "ResponseGetVersionReactor.h"
#include "ResponseCreateWorldReactor.h"
#include "ResponseGetSceneReactor.h"

#define CREATE_RESPONSE_REACTOR(_reactor) \
    auto reactor = std::make_shared<_reactor>(connection, message, nullptr); \
    reactor->SetMessageEncoder(mStreetGangPBRequestEncoder); \
    return reactor

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangpb;
using namespace streetgangclient;


StreetGangClientPBDecoder::StreetGangClientPBDecoder()
{
    mStreetGangPBRequestEncoder = std::make_shared<StreetGangPBRequestEncoder>();
}

StreetGangClientPBDecoder::~StreetGangClientPBDecoder()
{
}

std::shared_ptr<Reactor> StreetGangClientPBDecoder::Decode(std::istream& stream, Connection& connection)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    uint32_t size = 0;
    codedInputStream.ReadVarint32(&size);
    uint32_t previous = codedInputStream.PushLimit(size);
    streetgangpb::MessageHeader pbMessageHeader;
    bool result = pbMessageHeader.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previous);
    if (!result)
    {
        return nullptr;
    }

    if (pbMessageHeader.message_id() == "ErrorResponse")
    {
        codedInputStream.ReadVarint32(&size);
        uint32_t previousSize = codedInputStream.PushLimit(size);
        ErrorResponse pbErrorResponse;
        bool result = pbErrorResponse.ParseFromCodedStream(&codedInputStream);
        codedInputStream.PopLimit(previousSize);
        if (!result)
        {
            return nullptr;
        }

        auto message = std::make_shared<ResponseError>();
        message->TrackId.set(pbMessageHeader.track_id());
        message->Result.set(pbMessageHeader.result());
        message->RequestId.set(pbErrorResponse.request_id());
        message->ErrorMessage.set(pbErrorResponse.error_message());
        CREATE_RESPONSE_REACTOR(ResponseErrorReactor);
    }
    else if (pbMessageHeader.message_id() == "GetVersionResponse")
    {
        codedInputStream.ReadVarint32(&size);
        uint32_t previousSize = codedInputStream.PushLimit(size);
        GetVersionResponse pbGetVersionResponse;
        bool result = pbGetVersionResponse.ParseFromCodedStream(&codedInputStream);
        codedInputStream.PopLimit(previousSize);
        if (!result)
        {
            return nullptr;
        }

        auto message = std::make_shared<ResponseGetVersion>();
        message->TrackId.set(pbMessageHeader.track_id());
        message->Result.set(pbMessageHeader.result());
        message->Version.set(pbGetVersionResponse.version());
        CREATE_RESPONSE_REACTOR(ResponseGetVersionReactor);
    }
    else if (pbMessageHeader.message_id() == "CreateWorldResponse")
    {
        codedInputStream.ReadVarint32(&size);
        uint32_t previousSize = codedInputStream.PushLimit(size);
        CreateWorldResponse pbCreateWorldResponse;
        bool result = pbCreateWorldResponse.ParseFromCodedStream(&codedInputStream);
        codedInputStream.PopLimit(previousSize);
        if (!result)
        {
            return nullptr;
        }

        auto message = std::make_shared<ResponseCreateWorld>();
        message->TrackId.set(pbMessageHeader.track_id());
        message->Result.set(pbMessageHeader.result());
        message->WorldId.set(pbCreateWorldResponse.world_id());
        message->WorldName.set(pbCreateWorldResponse.world_name());
        CREATE_RESPONSE_REACTOR(ResponseCreateWorldReactor);
    }
    else if (pbMessageHeader.message_id() == "GetSceneResponse")
    {
        codedInputStream.ReadVarint32(&size);
        uint32_t previousSize = codedInputStream.PushLimit(size);
        GetSceneResponse pbGetSceneResponse;
        bool result = pbGetSceneResponse.ParseFromCodedStream(&codedInputStream);
        codedInputStream.PopLimit(previousSize);
        if (!result)
        {
            return nullptr;
        }

        auto message = std::make_shared<ResponseGetScene>();
        message->TrackId.set(pbMessageHeader.track_id());
        message->Result.set(pbMessageHeader.result());

        message->WorldId.set(pbGetSceneResponse.world_id());
        message->Rect->mX = pbGetSceneResponse.rect().x();
        message->Rect->mY = pbGetSceneResponse.rect().y();
        message->Rect->mW = pbGetSceneResponse.rect().w();
        message->Rect->mH = pbGetSceneResponse.rect().h();

        auto& pbItems = pbGetSceneResponse.items();
        for (auto& pbItem : pbItems)
        {
            message->Items->emplace_back(streetgangapi::Point<float>(pbItem.x(), pbItem.y(), pbItem.z()));
        }

        CREATE_RESPONSE_REACTOR(ResponseGetSceneReactor);
    }
    else
    {
        LOG("Unknown response: [TrackId=%s] [Request=%s]", pbMessageHeader.track_id().c_str(), pbMessageHeader.message_id().c_str());
        return nullptr;
    }
}
