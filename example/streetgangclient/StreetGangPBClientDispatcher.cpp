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

#include "StreetGangPBClientDispatcher.h"

#include "ResponseErrorReactor.h"
#include "ResponseGetVersionReactor.h"
#include "ResponseCreateWorldReactor.h"
#include "ResponseGetSceneReactor.h"

#include "PBResponseError.h"
#include "PBResponseGetVersion.h"
#include "PBResponseCreateWorld.h"
#include "PBResponseGetScene.h"


#define CREATE_RESPONSE_REACTOR(_id, _message, _reactor) \
    if (header.message_id() == _id) \
    { \
        auto message = std::make_shared<_message>(); \
        if (message->Decode(stream)) \
        { \
            auto reactor = std::make_shared<_reactor>(connection, message, mRequester); \
            return reactor; \
        } \
    }

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


StreetGangPBClientDispatcher::StreetGangPBClientDispatcher()
{
    mRequester = std::make_shared<streetgangapi::PBStreetGangRequester>();
}

StreetGangPBClientDispatcher::~StreetGangPBClientDispatcher()
{
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::Decode(std::istream& stream, Connection& connection)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    uint32_t size = 0;
    codedInputStream.ReadVarint32(&size);
    uint32_t previous = codedInputStream.PushLimit(size);
    streetgangpb::MessageHeader header;
    bool result = header.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previous);
    if (!result)
    {
        return nullptr;
    }

    CREATE_RESPONSE_REACTOR("ErrorResponse", streetgangapi::PBResponseError, ResponseErrorReactor)
    else CREATE_RESPONSE_REACTOR("GetVersionResponse", streetgangapi::PBResponseGetVersion, ResponseGetVersionReactor)
    else CREATE_RESPONSE_REACTOR("CreateWorldResponse", streetgangapi::PBResponseCreateWorld, ResponseCreateWorldReactor)
    else CREATE_RESPONSE_REACTOR("GetSceneResponse", streetgangapi::PBResponseGetScene, ResponseGetSceneReactor)
    else
    {
        LOG("Unknown response: [Request=%s]", header.message_id().c_str());
    }

    return nullptr;
}
