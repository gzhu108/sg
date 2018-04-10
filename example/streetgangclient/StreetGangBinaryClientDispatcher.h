#pragma once
#ifndef streetgangclient_StreetGangBinaryClientDispatcher
#define streetgangclient_StreetGangBinaryClientDispatcher

#include "MessageDispatcher.h"
#include "BinaryStreetGangRequester.h"


namespace streetgangclient
{
    class StreetGangBinaryClientDispatcher : public sg::microreactor::MessageDispatcher<int32_t>
    {
    public:
        StreetGangBinaryClientDispatcher();
        virtual ~StreetGangBinaryClientDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<sg::microreactor::Reactor> CreateErrorResponseReactor(std::istream& stream, std::shared_ptr<sg::microreactor::Connection> connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetVersionResponseReactor(std::istream& stream, std::shared_ptr<sg::microreactor::Connection> connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldResponseReactor(std::istream& stream, std::shared_ptr<sg::microreactor::Connection> connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetSceneResponseReactor(std::istream& stream, std::shared_ptr<sg::microreactor::Connection> connection);
    };
}

#endif // streetgangclient_StreetGangClientReactorFactory
