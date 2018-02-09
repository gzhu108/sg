#pragma once
#ifndef streetgangclient_StreetGangPBClientDispatcher
#define streetgangclient_StreetGangPBClientDispatcher

#include "MessageDecoder.h"
#include "PBStreetGangRequester.h"


namespace streetgangclient
{
    class StreetGangPBClientDispatcher : public sg::microreactor::MessageDecoder<std::string>
    {
    public:
        StreetGangPBClientDispatcher();
        virtual ~StreetGangPBClientDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<sg::microreactor::Reactor> CreateErrorResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetVersionResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetSceneResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);

    protected:
        std::shared_ptr<streetgangapi::PBStreetGangRequester> mRequester;
    };
}

#endif // streetgangclient_StreetGangPBClientDispatcher
