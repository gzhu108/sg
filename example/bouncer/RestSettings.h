#pragma once
#ifndef bouncer_RestSettings
#define bouncer_RestSettings

#include "RestApi.h"


namespace bouncer
{
    class RestSettings : public sg::microreactor::RestApi
    {
    public:
        RestSettings();
        virtual ~RestSettings();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection) override;

    protected:
        virtual void RespondWithError(sg::microreactor::Connection& connection, uint32_t statusCode, const std::string& statusText);
    };
}


#endif // bouncer_RestSettings
