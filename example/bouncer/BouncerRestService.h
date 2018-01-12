#pragma once
#ifndef bouncer_BouncerRestService
#define bouncer_BouncerRestService

#include "RestService.h"


namespace bouncer
{
    class BouncerRestService : public sg::microreactor::RestService
    {
    public:
        BouncerRestService(std::shared_ptr<sg::microreactor::Profile> profile);
        virtual ~BouncerRestService();

    public:
        virtual bool Initialize() override;

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateBouncerReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateSettingsReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
        virtual void RespondWithError(sg::microreactor::Connection& connection, uint32_t statusCode, const std::string& statusText);
    };
}


#endif // bouncer_BouncerRestService
