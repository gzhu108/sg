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
    RegisterMessageReactorFactory("ErrorResponse", std::bind(&StreetGangPBClientDispatcher::HandleErrorResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("GetVersionResponse", std::bind(&StreetGangPBClientDispatcher::HandleGetVersionResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("CreateWorldResponse", std::bind(&StreetGangPBClientDispatcher::HandleCreateWorldResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("GetSceneResponse", std::bind(&StreetGangPBClientDispatcher::HandleGetSceneResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
}

StreetGangPBClientDispatcher::~StreetGangPBClientDispatcher()
{
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::Decode(std::istream& stream, Connection& connection)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::MessageHeader header;
    bool result = header.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previousSize);
    if (!result)
    {
        return nullptr;
    }

    auto factory = GetMessageReactorFactory(header.message_id());
    if (factory == nullptr)
    {
        LOG("Unknown response: [Request=%s]", header.message_id().c_str());
        return nullptr;
    }

    return factory(codedInputStream, connection);
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::HandleErrorResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, Connection& connection)
{
    auto message = std::make_shared<PBResponseError>();
    if (message->Decode(codedInputStream))
    {
        auto trackedMessage = GetTrackedMessage(message->TrackId.cref());
        if (trackedMessage != nullptr && trackedMessage->Client.cref() != nullptr)
        {
            message->SetRequestTime(trackedMessage->GetRequestTime());
            auto client = trackedMessage->Client.cref();
            client->ProcessError(message);
        }
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::HandleGetVersionResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, Connection& connection)
{
    auto message = std::make_shared<PBResponseGetVersion>();
    if (message->Decode(codedInputStream))
    {
        auto trackedMessage = GetTrackedMessage(message->TrackId.cref());
        if (trackedMessage != nullptr && trackedMessage->Client.cref() != nullptr)
        {
            message->SetRequestTime(trackedMessage->GetRequestTime());
            auto client = std::static_pointer_cast<ResponseGetVersionReactor>(trackedMessage->Client.cref());
            client->Process(message);
        }
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::HandleCreateWorldResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, Connection& connection)
{
    auto message = std::make_shared<PBResponseCreateWorld>();
    if (message->Decode(codedInputStream))
    {
        auto trackedMessage = GetTrackedMessage(message->TrackId.cref());
        if (trackedMessage != nullptr && trackedMessage->Client.cref() != nullptr)
        {
            message->SetRequestTime(trackedMessage->GetRequestTime());
            auto client = std::static_pointer_cast<ResponseCreateWorldReactor>(trackedMessage->Client.cref());
            client->Process(message);
        }
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::HandleGetSceneResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, Connection& connection)
{
    auto message = std::make_shared<PBResponseGetScene>();
    if (message->Decode(codedInputStream))
    {
        auto trackedMessage = GetTrackedMessage(message->TrackId.cref());
        if (trackedMessage != nullptr && trackedMessage->Client.cref() != nullptr)
        {
            message->SetRequestTime(trackedMessage->GetRequestTime());
            auto client = std::static_pointer_cast<ResponseGetSceneReactor>(trackedMessage->Client.cref());
            client->Process(message);
        }
    }
    
    return nullptr;
}
