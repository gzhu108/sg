#pragma once
#ifndef bouncer_BouncerRestService
#define bouncer_BouncerRestService

#include "RestService.h"


namespace bouncer
{
    class BouncerRestService : public microreactor::RestService
    {
    public:
        BouncerRestService(std::shared_ptr<microreactor::Profile> profile);
        virtual ~BouncerRestService();

    public:
        virtual bool Initialize() override;

    protected:
        virtual std::shared_ptr<microreactor::Reactor> CreateBouncerReactor(std::shared_ptr<microreactor::RestMessage> message, microreactor::Connection& connection);
        virtual std::shared_ptr<microreactor::Reactor> CreateSettingsReactor(std::shared_ptr<microreactor::RestMessage> message, microreactor::Connection& connection);
    };
}


#endif // bouncer_BouncerRestService
