#pragma once
#ifndef worldserver_WorldDispatcher
#define worldserver_WorldDispatcher

#include "MessageDispatcher.h"
#include "ReactorFactoryContainer.h"
#include "WorldResponder.h"


namespace worldserver
{
    class WorldDispatcher
        : public microreactor::MessageDispatcher
        , public microreactor::ReactorFactoryContainer<int32_t>
    {
    public:
        WorldDispatcher();
        virtual ~WorldDispatcher();

    public:
        virtual std::shared_ptr<microreactor::Reactor> Decode(std::istream& stream, microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<microreactor::Reactor> CreateCreateWorldReactor(std::istream& stream, microreactor::Connection& connection);
        std::shared_ptr<microreactor::Reactor> CreateGetWorldReactor(std::istream& stream, microreactor::Connection& connection);
    };
}

#endif // worldserver_WorldDispatcher
