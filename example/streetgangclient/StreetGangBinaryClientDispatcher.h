#pragma once
#ifndef streetgangclient_StreetGangBinaryClientDispatcher
#define streetgangclient_StreetGangBinaryClientDispatcher

#include "MessageDispatcher.h"
#include "ReactorFactoryContainer.h"


namespace streetgangclient
{
    class StreetGangBinaryClientDispatcher
        : public microreactor::MessageDispatcher
        , public microreactor::ReactorFactoryContainer<int32_t>
    {
    public:
        StreetGangBinaryClientDispatcher();
        virtual ~StreetGangBinaryClientDispatcher();

    public:
        virtual std::shared_ptr<microreactor::Reactor> Decode(std::istream& stream, microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<microreactor::Reactor> HandleErrorResponseReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> HandleGetVersionResponseReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> HandleCreateWorldResponseReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> HandleGetSceneResponseReactor(std::istream& stream, microreactor::Connection& connection);
    };
}

#endif // streetgangclient_StreetGangClientReactorFactory
