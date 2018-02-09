#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/cpp/MessageHeader.pb.h"
#include "protobuf/cpp/ErrorResponse.pb.h"

#include "StreetGangPBDispatcher.h"
#include "StreetGangSessionManager.h"

#include "RequestGetVersionReactor.h"
#include "RequestByebyeReactor.h"
#include "RequestCreateWorldReactor.h"
#include "RequestGetSceneReactor.h"

#include "PBRequestByebye.h"
#include "PBRequestGetVersion.h"
#include "PBRequestCreateWorld.h"
#include "PBRequestGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangpb;
using namespace streetgangserver;


StreetGangPBDispatcher::StreetGangPBDispatcher()
{
    RegisterMessageReactorFactory("ByebyeRequest", std::bind(&StreetGangPBDispatcher::CreateByebyeReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("GetVersionRequest", std::bind(&StreetGangPBDispatcher::CreateGetVersionReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("CreateWorldRequest", std::bind(&StreetGangPBDispatcher::CreateCreateWorldReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("GetSceneRequest", std::bind(&StreetGangPBDispatcher::CreateGetSceneReactor, this, std::placeholders::_1, std::placeholders::_2));
}

StreetGangPBDispatcher::~StreetGangPBDispatcher()
{
}

std::shared_ptr<Reactor> StreetGangPBDispatcher::Decode(std::istream& stream, Connection& connection)
{
    google::protobuf::io::IstreamInputStream istreamInputStream(&stream);
    google::protobuf::io::CodedInputStream codedInputStream(&istreamInputStream);

    google::protobuf::io::CodedInputStream::Limit previous = codedInputStream.ReadLengthAndPushLimit();
    streetgangpb::MessageHeader header;
    bool result = header.ParseFromCodedStream(&codedInputStream);
    codedInputStream.PopLimit(previous);
    if (!result)
    {
        return nullptr;
    }

    auto factory = GetMessageReactorFactory(header.message_id());
    if (factory == nullptr)
    {
        LOG("Unknown request: [Request=%s]", header.message_id().c_str());
        auto responder = std::make_shared<PBStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorNotImplemented, static_cast<int32_t>(streetgangapi::ID::Unknown), header.message_id() + " Unknown request");
        return nullptr;
    }

    return factory(stream, connection);
}

std::shared_ptr<Reactor> StreetGangPBDispatcher::CreateByebyeReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<PBRequestByebye>();
    if (message->Decode(stream))
    {
        return std::make_shared<RequestByebyeReactor>(connection, message, std::make_shared<PBStreetGangResponder>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBDispatcher::CreateGetVersionReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<PBRequestGetVersion>();
    if (message->Decode(stream))
    {
        return std::make_shared<RequestGetVersionReactor>(connection, message, std::make_shared<PBStreetGangResponder>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBDispatcher::CreateCreateWorldReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<PBRequestCreateWorld>();
    if (message->Decode(stream))
    {
        return std::make_shared<RequestCreateWorldReactor>(connection, message, std::make_shared<PBStreetGangResponder>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBDispatcher::CreateGetSceneReactor(std::istream& stream, Connection& connection)
{
    auto message = std::make_shared<PBRequestGetScene>();
    if (message->Decode(stream))
    {
        auto reactor = std::make_shared<RequestGetSceneReactor>(connection, message, std::make_shared<PBStreetGangResponder>(connection));
        reactor->SetSession(StreetGangSessionManager::GetInstance().GetSession(message->WorldId.cref()));
        return reactor;
    }

    return nullptr;
}
