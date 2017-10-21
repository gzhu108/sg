#pragma once
#ifndef sg_microreactor_Microservice
#define sg_microreactor_Microservice

#include "Service.h"
#include "Profile.h"


namespace sg { namespace microreactor
{
    class Microservice : public Service
    {
    public:
        Microservice();
        explicit Microservice(std::shared_ptr<Profile> profile);
        Microservice(std::shared_ptr<Host> host, std::shared_ptr<Profile> profile);
        virtual ~Microservice();

    public:
        virtual bool Start() override;
        virtual bool Stop() override;
        
        std::shared_ptr<Profile> GetProfile() { return mProfile; }

    protected:
        virtual bool Initialize();
        virtual void OnConnectionMade(const std::shared_ptr<Connection>& connection);

    protected:
        std::shared_ptr<Profile> mProfile;
        std::shared_ptr<Listener> mListener;
    };
}}

#endif // sg_microreactor_Microservice
