#pragma once
#ifndef streetgangserver_WorldClientDispatcher
#define streetgangserver_WorldClientDispatcher

#include "MessageDispatcher.h"


namespace streetgangserver
{
    class WorldClientDispatcher : public sg::microreactor::MessageDispatcher<int32_t>
    {
    public:
        WorldClientDispatcher();
        virtual ~WorldClientDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<sg::microreactor::Reactor> CreateErrorResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetWorldResponseReactor(std::istream& stream, sg::microreactor::Connection& connection);
    };
}

#endif // streetgangserver_WorldClientDispatcher