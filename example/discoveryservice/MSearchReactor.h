#pragma once
#ifndef sg_service_MSearchReactor
#define sg_service_MSearchReactor

#include "RestReactor.h"
#include "RestRequest.h"
#include "ServiceDescription.h"


namespace sg { namespace service
{
    class MSearchReactor : public sg::microreactor::RestReactor<sg::microreactor::RestRequest>
    {
    public:
        MSearchReactor(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~MSearchReactor();

        PROPERTY(Description, ServiceDescription);

    public:
        virtual bool Process() override;
    };
}}


#endif // sg_service_MSearchReactor
