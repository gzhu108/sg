#pragma once
#ifndef sg_microreactor_RestService
#define sg_microreactor_RestService

#include "Microservice.h"
#include "RestDispatcher.h"


namespace sg { namespace microreactor
{
    class RestService : public Microservice
    {
    public:
        RestService(const std::string& hostName, uint16_t port);
        explicit RestService(std::shared_ptr<Profile> profile);
        explicit RestService(std::shared_ptr<Endpoint> endpoint);
        virtual ~RestService();

    public:
        virtual bool Initialize() override;

    protected:
        std::shared_ptr<RestDispatcher> mRestDispatcher;
    };
}}


#endif // sg_microreactor_RestService
