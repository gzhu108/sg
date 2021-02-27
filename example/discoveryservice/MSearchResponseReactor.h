#pragma once
#ifndef sg_service_MSearchResponseReactor
#define sg_service_MSearchResponseReactor

#include "RestReactor.h"
#include "RestResponse.h"
#include "ServiceDescription.h"


namespace sg { namespace service
{
    class MSearchResponseReactor : public microreactor::RestReactor<microreactor::RestResponse>
    {
    public:
        MSearchResponseReactor(microreactor::Connection& connection, std::shared_ptr<microreactor::RestResponse> response);
        virtual ~MSearchResponseReactor();

        PROPERTY(ServiceType, std::string);

        microreactor::Signal<ServiceDescription>& ServiceFound = mServiceFound;

    public:
        virtual bool Process() override;

    protected:
        microreactor::Emittable<ServiceDescription> mServiceFound;
    };
}}


#endif // sg_service_MSearchResponseReactor
