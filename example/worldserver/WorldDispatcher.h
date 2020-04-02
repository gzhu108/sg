#pragma once
#ifndef worldserver_WorldDispatcher
#define worldserver_WorldDispatcher

#include "MessageDispatcher.h"
#include "ReactorFactoryContainer.h"
#include "WorldResponder.h"


namespace worldserver
{
    class WorldDispatcher
        : public sg::microreactor::MessageDispatcher
        , public sg::microreactor::ReactorFactoryContainer<int32_t>
    {
    public:
        WorldDispatcher();
        virtual ~WorldDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldReactor(std::istream& stream, sg::microreactor::Connection& connection);
        std::shared_ptr<sg::microreactor::Reactor> CreateGetWorldReactor(std::istream& stream, sg::microreactor::Connection& connection);
    };
}

#endif // worldserver_WorldDispatcher
