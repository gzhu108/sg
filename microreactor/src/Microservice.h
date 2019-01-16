#pragma once
#ifndef sg_microreactor_Microservice
#define sg_microreactor_Microservice

#include "Service.h"
#include "Profile.h"
#include "Endpoint.h"


namespace sg { namespace microreactor
{
    class Microservice : public Service
    {
    public:
        Microservice();
        explicit Microservice(std::shared_ptr<Profile> profile);
        explicit Microservice(std::shared_ptr<Endpoint> endpoint);
        virtual ~Microservice();

    public:
        virtual bool Start() override;
        virtual bool Stop() override;

        virtual uint64_t SendAllConnections(std::iostream& stream);
        virtual uint64_t SendAllConnections(const char* buffer, int32_t length);

    protected:
        virtual bool Initialize();
        virtual void OnConnectionMade(const std::shared_ptr<Connection>& connection);

    protected:
        std::shared_ptr<Profile> mProfile;
        std::shared_ptr<Endpoint> mEndpoint;
    };
}}

#endif // sg_microreactor_Microservice
