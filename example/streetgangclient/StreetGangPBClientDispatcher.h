#pragma once
#ifndef streetgangclient_StreetGangPBClientDispatcher
#define streetgangclient_StreetGangPBClientDispatcher

#include "google/protobuf/io/coded_stream.h"
#include "MessageDispatcher.h"
#include "ReactorFactoryContainer.h"


namespace streetgangclient
{
    class StreetGangPBClientDispatcher
        : public sg::microreactor::MessageDispatcher
        , public sg::microreactor::ReactorFactoryContainer<std::string, google::protobuf::io::CodedInputStream>
    {
    public:
        StreetGangPBClientDispatcher();
        virtual ~StreetGangPBClientDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<sg::microreactor::Reactor> HandleErrorResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> HandleGetVersionResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> HandleCreateWorldResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> HandleGetSceneResponseReactor(google::protobuf::io::CodedInputStream& codedInputStream, sg::microreactor::Connection& connection);
    };
}

#endif // streetgangclient_StreetGangPBClientDispatcher
