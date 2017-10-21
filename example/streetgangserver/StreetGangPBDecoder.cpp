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

#include "StreetGangPBDecoder.h"
#include "StreetGangIds.h"
#include "StreetGangSessionManager.h"

#include "RequestErrorReactor.h"
#include "RequestGetVersionReactor.h"
#include "RequestByebyeReactor.h"
#include "RequestCreateWorldReactor.h"
#include "RequestGetSceneReactor.h"

#define CREATE_REQUEST_REACTOR(_reactor) \
    auto reactor = std::make_shared<_reactor>(connection, message); \
    reactor->SetMessageEncoder(mStreetGangPBResponseEncoder); \
    return reactor

#define CREATE_SESSION_REQUEST_REACTOR(_reactor) \
    auto reactor = std::make_shared<_reactor>(connection, message); \
    reactor->SetMessageEncoder(mStreetGangPBResponseEncoder); \
    auto session = StreetGangSessionManager::GetInstance().GetSession(message->WorldId.cref()); \
    reactor->SetSession(session); \
    return reactor

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangpb;
using namespace streetgangserver;


StreetGangPBDecoder::StreetGangPBDecoder()
{
    mStreetGangPBResponseEncoder = std::make_shared<StreetGangPBResponseEncoder>();
}

StreetGangPBDecoder::~StreetGangPBDecoder()
{
}

std::shared_ptr<Reactor> StreetGangPBDecoder::Decode(std::istream& stream, Connection& connection)
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

    if (pbMessageHeader.message_id() == "ByebyeRequest")
    {
        auto message = std::make_shared<RequestByebye>();
        message->TrackId.set(pbMessageHeader.track_id());
        message->Result.set(pbMessageHeader.result());
        CREATE_REQUEST_REACTOR(RequestByebyeReactor);
    }
    else if (pbMessageHeader.message_id() == "GetVersionRequest")
    {
        auto message = std::make_shared<RequestGetVersion>();
        message->TrackId.set(pbMessageHeader.track_id());
        message->Result.set(pbMessageHeader.result());
        CREATE_REQUEST_REACTOR(RequestGetVersionReactor);
    }
    else if (pbMessageHeader.message_id() == "CreateWorldRequest")
    {
        codedInputStream.ReadVarint32(&size);
        uint32_t previousSize = codedInputStream.PushLimit(size);
        CreateWorldRequest pbCresateWorldRequest;
        bool result = pbCresateWorldRequest.ParseFromCodedStream(&codedInputStream);
        codedInputStream.PopLimit(previousSize);
        if (!result)
        {
            return nullptr;
        }

        auto message = std::make_shared<RequestCreateWorld>();
        message->TrackId.set(pbMessageHeader.track_id());
        message->Result.set(pbMessageHeader.result());
        message->WorldName.set(pbCresateWorldRequest.world_name());
        CREATE_REQUEST_REACTOR(RequestCreateWorldReactor);
    }
    else if (pbMessageHeader.message_id() == "GetSceneRequest")
    {
        codedInputStream.ReadVarint32(&size);
        uint32_t previousSize = codedInputStream.PushLimit(size);
        GetSceneRequest pbGetSceneRequest;
        bool result = pbGetSceneRequest.ParseFromCodedStream(&codedInputStream);
        codedInputStream.PopLimit(previousSize);
        if (!result)
        {
            return nullptr;
        }

        auto message = std::make_shared<RequestGetScene>();
        message->TrackId.set(pbMessageHeader.track_id());
        message->Result.set(pbMessageHeader.result());

        message->WorldId.set(pbGetSceneRequest.world_id());
        message->Rect->mX = pbGetSceneRequest.rect().x();
        message->Rect->mY = pbGetSceneRequest.rect().y();
        message->Rect->mW = pbGetSceneRequest.rect().w();
        message->Rect->mH = pbGetSceneRequest.rect().h();
        CREATE_SESSION_REQUEST_REACTOR(RequestGetSceneReactor);
    }
    else
    {
        LOG("Unknown request: [TrackId=%s] [Request=%s]", pbMessageHeader.track_id().c_str(), pbMessageHeader.message_id().c_str());
        auto message = std::make_shared<ResponseError>();
        message->Result.set((int32_t)ResultCode::ErrorNotImplemented);
        message->ErrorMessage.set("Unknown Request: " + pbMessageHeader.message_id());
        auto reactor = std::make_shared<RequestErrorReactor>(connection, message);
        reactor->SetMessageEncoder(mStreetGangPBResponseEncoder);
        return reactor;
    }

    return nullptr;
}
