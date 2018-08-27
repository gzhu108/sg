#pragma once
#ifndef sg_service_ServiceDescription
#define sg_service_ServiceDescription

#include "Uuid.h"


namespace sg { namespace service
{
    class ServiceDescription
    {
    public:
        ServiceDescription()
        {
        }

        ServiceDescription(const ServiceDescription& description)
        {
            NotifyMaxAge.set(description.NotifyMaxAge.cref());
            Location.set(description.Location.cref());
            ServerInfo.set(description.ServerInfo.cref());
            Usn.set(description.Usn.cref());
            ServiceType.set(description.ServiceType.cref());
            Date.set(description.Date.cref());
        }

        PROPERTY(NotifyMaxAge, uint32_t, 0);
        PROPERTY(Location, std::string);
        PROPERTY(ServerInfo, std::string);
        PROPERTY(Usn, std::string);
        PROPERTY(ServiceType, std::string);
        PROPERTY(Date, std::string);

    public:
        ServiceDescription& operator =(const ServiceDescription& description)
        {
            NotifyMaxAge.set(description.NotifyMaxAge.cref());
            Location.set(description.Location.cref());
            ServerInfo.set(description.ServerInfo.cref());
            Usn.set(description.Usn.cref());
            ServiceType.set(description.ServiceType.cref());
            Date.set(description.Date.cref());
            return *this;
        }

        bool operator <(const ServiceDescription& description) const
        {
            return Usn.cref() < description.Usn.cref();
        }
    };
}}

#endif // sg_service_ServiceDescription

