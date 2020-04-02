#pragma once
#ifndef streetgangserver_StreetGangPBDispatcher
#define streetgangserver_StreetGangPBDispatcher

#include "google/protobuf/io/coded_stream.h"
#include "MessageDispatcher.h"
#include "ReactorFactoryContainer.h"
#include "PBStreetGangResponder.h"


namespace streetgangserver
{
    class StreetGangPBDispatcher
        : public sg::microreactor::MessageDispatcher
        , public sg::microreactor::ReactorFactoryContainer<std::string, google::protobuf::io::CodedInputStream>
    {
    public:
        StreetGangPBDispatcher();
        virtual ~StreetGangPBDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<sg::microreactor::Reactor> CreateByebyeReactor(google::protobuf::io::CodedInputStream& codedInputStream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetVersionReactor(google::protobuf::io::CodedInputStream& codedInputStream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldReactor(google::protobuf::io::CodedInputStream& codedInputStream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetSceneReactor(google::protobuf::io::CodedInputStream& codedInputStream, sg::microreactor::Connection& connection);

    protected:
        std::shared_ptr<streetgangapi::PBStreetGangResponder> mResponder;
    };
}

#endif // streetgangserver_StreetGangPBDispatcher
