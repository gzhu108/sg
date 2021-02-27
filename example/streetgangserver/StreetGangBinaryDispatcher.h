#pragma once
#ifndef streetgangserver_StreetGangBinaryDispatcher
#define streetgangserver_StreetGangBinaryDispatcher

#include "MessageDispatcher.h"
#include "ReactorFactoryContainer.h"
#include "BinaryStreetGangResponder.h"


namespace streetgangserver
{
    class StreetGangBinaryDispatcher
        : public microreactor::MessageDispatcher
        , public microreactor::ReactorFactoryContainer<int32_t>
    {
    public:
        StreetGangBinaryDispatcher();
        virtual ~StreetGangBinaryDispatcher();

    public:
        virtual std::shared_ptr<microreactor::Reactor> Decode(std::istream& stream, microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<microreactor::Reactor> CreateByebyeReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> CreateGetVersionReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> CreateCreateWorldReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> CreateGetSceneReactor(std::istream& stream, microreactor::Connection& connection);
    };
}

#endif // streetgangserver_StreetGangBinaryDispatcher
