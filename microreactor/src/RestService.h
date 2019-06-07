#pragma once
#ifndef sg_microreactor_RestService
#define sg_microreactor_RestService

#include "Service.h"
#include "RestDispatcher.h"


namespace sg { namespace microreactor
{
    class RestService : public Service
    {
    public:
        RestService();
        RestService(const std::string& hostAddress, uint16_t port);
        explicit RestService(std::shared_ptr<Profile> profile);
        explicit RestService(std::shared_ptr<Endpoint> endpoint);
        virtual ~RestService();

    public:
        virtual bool Initialize() override;

    protected:
        virtual void OnConnectionMade(const std::shared_ptr<Connection>& connection) override;

    protected:
        std::shared_ptr<RestDispatcher> mRestDispatcher;
    };
}}


#endif // sg_microreactor_RestService
