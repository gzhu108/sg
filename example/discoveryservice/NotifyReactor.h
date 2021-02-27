#pragma once
#ifndef sg_service_NotifyReactor
#define sg_service_NotifyReactor

#include "RestReactor.h"
#include "RestRequest.h"
#include "Uuid.h"


namespace sg { namespace service
{
    class NotifyReactor : public microreactor::RestReactor<microreactor::RestRequest>
    {
    public:
        NotifyReactor(microreactor::Connection& connection, std::shared_ptr<microreactor::RestRequest> request);
        virtual ~NotifyReactor();

        PROPERTY(ServiceType, std::string);

        microreactor::Signal<microreactor::Uuid>& Byebye = mByebye;

    public:
        virtual bool Process() override;

    protected:
        void UnicastMSearch(const std::string& unicastAddress, uint16_t port);

    protected:
        microreactor::Emittable<microreactor::Uuid> mByebye;
    };
}}


#endif // sg_service_NotifyReactor
