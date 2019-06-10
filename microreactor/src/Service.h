#pragma once
#ifndef sg_microreactor_Service
#define sg_microreactor_Service

#include "Profile.h"
#include "Endpoint.h"
#include "NetworkUtility.h"


namespace sg { namespace microreactor
{
    class Service
    {
    public:
        Service();
        explicit Service(std::shared_ptr<Endpoint> endpoint);
        virtual ~Service();

    public:
        virtual bool Start();
        virtual bool Stop();
        virtual void Restart();

        virtual int64_t SendAllConnections(std::iostream& stream);
        virtual int64_t SendAllConnections(const char* buffer, int32_t length);

    protected:
        virtual bool Initialize();
        virtual void OnConnectionMade(const std::shared_ptr<Connection>& connection);

    protected:
        std::shared_ptr<Endpoint> mEndpoint;
    };
}}

#endif // sg_microreactor_Service
