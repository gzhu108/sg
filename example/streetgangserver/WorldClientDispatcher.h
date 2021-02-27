#pragma once
#ifndef streetgangserver_WorldClientDispatcher
#define streetgangserver_WorldClientDispatcher

#include "MessageDispatcher.h"
#include "ReactorFactoryContainer.h"


namespace streetgangserver
{
    class WorldClientDispatcher
        : public microreactor::MessageDispatcher
        , public microreactor::ReactorFactoryContainer<int32_t>
    {
    public:
        WorldClientDispatcher();
        virtual ~WorldClientDispatcher();

    public:
        virtual std::shared_ptr<microreactor::Reactor> Decode(std::istream& stream, microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<microreactor::Reactor> HandleErrorResponseReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> HandleCreateWorldResponseReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> HandleGetWorldResponseReactor(std::istream& stream, microreactor::Connection& connection);
    };
}

#endif // streetgangserver_WorldClientDispatcher
