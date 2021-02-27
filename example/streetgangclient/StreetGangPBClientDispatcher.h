#pragma once
#ifndef streetgangclient_StreetGangPBClientDispatcher
#define streetgangclient_StreetGangPBClientDispatcher

#include "google/protobuf/io/coded_stream.h"
#include "MessageDispatcher.h"
#include "ReactorFactoryContainer.h"


namespace streetgangclient
{
    class StreetGangPBClientDispatcher
        : public microreactor::MessageDispatcher
        , public microreactor::ReactorFactoryContainer<std::string, google::protobuf::io::CodedInputStream>
    {
    public:
        StreetGangPBClientDispatcher();
        virtual ~StreetGangPBClientDispatcher();

    public:
        virtual std::shared_ptr<microreactor::Reactor> Decode(std::istream& stream, microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<microreactor::Reactor> HandleErrorResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> HandleGetVersionResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> HandleCreateWorldResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> HandleGetSceneResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, microreactor::Connection& connection);
    };
}

#endif // streetgangclient_StreetGangPBClientDispatcher
