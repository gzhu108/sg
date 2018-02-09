#pragma once
#ifndef streetgangserver_StreetGangPBDispatcher
#define streetgangserver_StreetGangPBDispatcher

#include "MessageDecoder.h"
#include "PBStreetGangResponder.h"


namespace streetgangserver
{
    class StreetGangPBDispatcher : public sg::microreactor::MessageDecoder<std::string>
    {
    public:
        StreetGangPBDispatcher();
        virtual ~StreetGangPBDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<sg::microreactor::Reactor> CreateByebyeReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetVersionReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetSceneReactor(std::istream& stream, sg::microreactor::Connection& connection);

    protected:
        std::shared_ptr<streetgangapi::PBStreetGangResponder> mResponder;
    };
}

#endif // streetgangserver_StreetGangPBDispatcher
