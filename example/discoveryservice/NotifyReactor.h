#pragma once
#ifndef sg_service_NotifyReactor
#define sg_service_NotifyReactor

#include "RestReactor.h"
#include "RestRequest.h"
#include "Uuid.h"


namespace sg { namespace service
{
    class NotifyReactor : public sg::microreactor::RestReactor<sg::microreactor::RestRequest>
    {
    public:
        NotifyReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~NotifyReactor();

        PROPERTY(ServiceType, std::string);

        sg::microreactor::Signal<sg::microreactor::Uuid>& Byebye = mByebye;

    public:
        virtual bool Process() override;

    protected:
        void UnicastMSearch(const std::string& unicastAddress, uint16_t port);

    protected:
        sg::microreactor::Emittable<sg::microreactor::Uuid> mByebye;
    };
}}


#endif // sg_service_NotifyReactor
