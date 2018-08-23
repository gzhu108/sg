#pragma once
#ifndef sg_service_NotifyReactor
#define sg_service_NotifyReactor

#include "RestReactor.h"
#include "RestRequest.h"


namespace sg { namespace service
{
    class NotifyReactor : public sg::microreactor::RestReactor<sg::microreactor::RestRequest>
    {
    public:
        NotifyReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~NotifyReactor();

        PROPERTY(NotifyMaxAge, uint32_t, 0);
        PROPERTY(Location, std::string);
        PROPERTY(ServerInfo, std::string);
        PROPERTY(Usn, std::string);
        PROPERTY(ServiceType, std::string);

    public:
        virtual bool Process() override;
    };
}}


#endif // sg_service_NotifyReactor
