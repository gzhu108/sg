#pragma once
#ifndef streetgangclient_StreetGangBinaryClientDispatcher
#define streetgangclient_StreetGangBinaryClientDispatcher

#include "MessageDispatcher.h"


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
        std::shared_ptr<sg::microreactor::Reactor> HandleErrorResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> HandleGetVersionResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> HandleCreateWorldResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> HandleGetSceneResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
    };
}

#endif // streetgangclient_StreetGangClientReactorFactory
