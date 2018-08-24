#pragma once
#ifndef sg_service_MSearchResponseReactor
#define sg_service_MSearchResponseReactor

#include "RestReactor.h"
#include "RestResponse.h"
#include "ServiceDescription.h"


namespace sg { namespace service
{
    class MSearchResponseReactor : public sg::microreactor::RestReactor<sg::microreactor::RestResponse>
    {
    public:
        MSearchResponseReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<sg::microreactor::RestResponse> response);
        virtual ~MSearchResponseReactor();

        PROPERTY(ServiceType, std::string);

        sg::microreactor::Signal<ServiceDescription>& ServiceFound = mServiceFound;

    public:
        virtual bool Process() override;

    protected:
        sg::microreactor::Emittable<ServiceDescription> mServiceFound;
    };
}}


#endif // sg_service_MSearchResponseReactor
