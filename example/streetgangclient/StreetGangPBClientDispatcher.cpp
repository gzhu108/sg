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
    RegisterMessageReactorFactory("ErrorResponse", std::bind(&StreetGangPBClientDispatcher::CreateErrorResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("GetVersionResponse", std::bind(&StreetGangPBClientDispatcher::CreateGetVersionResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("CreateWorldResponse", std::bind(&StreetGangPBClientDispatcher::CreateCreateWorldResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
    RegisterMessageReactorFactory("GetSceneResponse", std::bind(&StreetGangPBClientDispatcher::CreateGetSceneResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
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

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::CreateErrorResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, Connection& connection)
{
    auto message = std::make_shared<PBResponseError>();
    if (message->Decode(codedInputStream))
    {
        return std::make_shared<ResponseErrorReactor>(connection, message, std::make_shared<streetgangapi::PBStreetGangRequester>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::CreateGetVersionResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, Connection& connection)
{
    auto message = std::make_shared<PBResponseGetVersion>();
    if (message->Decode(codedInputStream))
    {
        return std::make_shared<ResponseGetVersionReactor>(connection, message, std::make_shared<streetgangapi::PBStreetGangRequester>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::CreateCreateWorldResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, Connection& connection)
{
    auto message = std::make_shared<PBResponseCreateWorld>();
    if (message->Decode(codedInputStream))
    {
        return std::make_shared<ResponseCreateWorldReactor>(connection, message, std::make_shared<streetgangapi::PBStreetGangRequester>(connection));
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangPBClientDispatcher::CreateGetSceneResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, Connection& connection)
{
    auto message = std::make_shared<PBResponseGetScene>();
    if (message->Decode(codedInputStream))
    {
        return std::make_shared<ResponseGetSceneReactor>(connection, message, std::make_shared<streetgangapi::PBStreetGangRequester>(connection));
    }
    
    return nullptr;
}
