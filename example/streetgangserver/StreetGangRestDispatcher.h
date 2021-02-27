#pragma once
#ifndef streetgangserver_StreetGangRestDispatcher
#define streetgangserver_StreetGangRestDispatcher

#include "RestDispatcher.h"


namespace streetgangserver
{
    class StreetGangRestDispatcher : public microreactor::RestDispatcher
    {
    public:
        explicit StreetGangRestDispatcher();
        virtual ~StreetGangRestDispatcher();

    protected:
        virtual std::shared_ptr<microreactor::Reactor> CreateGetVersionReactor(std::shared_ptr<microreactor::RestMessage> message, microreactor::Connection& connection);
        virtual std::shared_ptr<microreactor::Reactor> CreateCreateWorldReactor(std::shared_ptr<microreactor::RestMessage> message, microreactor::Connection& connection);
        virtual std::shared_ptr<microreactor::Reactor> CreateGetSceneReactor(std::shared_ptr<microreactor::RestMessage> message, microreactor::Connection& connection);
    };
}


#endif // streetgangserver_StreetGangRestDispatcher
