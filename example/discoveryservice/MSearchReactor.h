#pragma once
#ifndef sg_service_MSearchReactor
#define sg_service_MSearchReactor

#include "RestReactor.h"
#include "RestMessage.h"


namespace sg { namespace service
{
    class MSearchReactor : public sg::microreactor::RestReactor<sg::microreactor::RestMessage>
    {
    public:
        MSearchReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~MSearchReactor();

        PROPERTY(NotifyMaxAge, uint32_t, 0);
        PROPERTY(Location, std::string);
        PROPERTY(ServerInfo, std::string);
        PROPERTY(Usn, std::string);
        PROPERTY(ServiceType, std::string);

    public:
        virtual bool Process() override;
    };
}}


#endif // sg_service_MSearchReactor
