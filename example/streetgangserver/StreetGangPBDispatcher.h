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
        : public microreactor::MessageDispatcher
        , public microreactor::ReactorFactoryContainer<std::string, google::protobuf::io::CodedInputStream>
    {
    public:
        StreetGangPBDispatcher();
        virtual ~StreetGangPBDispatcher();

    public:
        virtual std::shared_ptr<microreactor::Reactor> Decode(std::istream& stream, microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<microreactor::Reactor> CreateByebyeReactor(google::protobuf::io::CodedInputStream& codedInputStream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> CreateGetVersionReactor(google::protobuf::io::CodedInputStream& codedInputStream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> CreateCreateWorldReactor(google::protobuf::io::CodedInputStream& codedInputStream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> CreateGetSceneReactor(google::protobuf::io::CodedInputStream& codedInputStream, microreactor::Connection& connection);

    protected:
        std::shared_ptr<streetgangapi::PBStreetGangResponder> mResponder;
    };
}

#endif // streetgangserver_StreetGangPBDispatcher
