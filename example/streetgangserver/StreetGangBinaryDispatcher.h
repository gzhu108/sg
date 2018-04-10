#pragma once
#ifndef streetgangserver_StreetGangBinaryDispatcher
#define streetgangserver_StreetGangBinaryDispatcher

#include "MessageDispatcher.h"
#include "BinaryStreetGangResponder.h"


namespace streetgangserver
{
    class StreetGangBinaryDispatcher : public sg::microreactor::MessageDispatcher<int32_t>
    {
    public:
        StreetGangBinaryDispatcher();
        virtual ~StreetGangBinaryDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<sg::microreactor::Reactor> CreateByebyeReactor(std::istream& stream, std::shared_ptr<sg::microreactor::Connection> connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetVersionReactor(std::istream& stream, std::shared_ptr<sg::microreactor::Connection> connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldReactor(std::istream& stream, std::shared_ptr<sg::microreactor::Connection> connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetSceneReactor(std::istream& stream, std::shared_ptr<sg::microreactor::Connection> connection);
    };
}

#endif // streetgangserver_StreetGangBinaryDispatcher
