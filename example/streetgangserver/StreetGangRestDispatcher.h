#pragma once
#ifndef streetgangserver_StreetGangRestDispatcher
#define streetgangserver_StreetGangRestDispatcher

#include "RestDispatcher.h"


namespace streetgangserver
{
    class StreetGangRestDispatcher : public sg::microreactor::RestDispatcher
    {
    public:
        explicit StreetGangRestDispatcher();
        virtual ~StreetGangRestDispatcher();

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGetVersionReactor(std::shared_ptr<sg::microreactor::RestMessage> message, sg::microreactor::Connection& connection);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldReactor(std::shared_ptr<sg::microreactor::RestMessage> message, sg::microreactor::Connection& connection);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGetSceneReactor(std::shared_ptr<sg::microreactor::RestMessage> message, sg::microreactor::Connection& connection);
    };
}


#endif // streetgangserver_StreetGangRestDispatcher
