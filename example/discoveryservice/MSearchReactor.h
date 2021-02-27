#pragma once
#ifndef sg_service_MSearchReactor
#define sg_service_MSearchReactor

#include "RestReactor.h"
#include "RestRequest.h"
#include "ServiceDescription.h"


namespace sg { namespace service
{
    class MSearchReactor : public microreactor::RestReactor<microreactor::RestRequest>
    {
    public:
        MSearchReactor(microreactor::Connection& connection, std::shared_ptr<microreactor::RestRequest> request);
        virtual ~MSearchReactor();

        PROPERTY(Description, ServiceDescription);

    public:
        virtual bool Process() override;
    };
}}


#endif // sg_service_MSearchReactor
