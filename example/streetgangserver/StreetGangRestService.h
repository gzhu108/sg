#pragma once
#ifndef streetgangserver_StreetGangRestService
#define streetgangserver_StreetGangRestService

#include "RestService.h"


namespace streetgangserver
{
    class StreetGangRestService : public sg::microreactor::RestService
    {
    public:
        StreetGangRestService(std::shared_ptr<sg::microreactor::Endpoint> endpoint, std::shared_ptr<sg::microreactor::Profile> profile);
        virtual ~StreetGangRestService();

    public:
        virtual bool Initialize() override;

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGetVersionReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateCreateWorldReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateGetSceneReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection);
    };
}


#endif // streetgangserver_StreetGangRestService
