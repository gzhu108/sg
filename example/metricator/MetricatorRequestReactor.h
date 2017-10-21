#pragma once
#ifndef metricator_MetricatorRequestReactor
#define metricator_MetricatorRequestReactor

#include "MessageReactor.h"
#include "MetricatorRequest.h"


namespace metricator
{
    class MetricatorRequestReactor : public sg::microreactor::MessageReactor<metricatorapi::MetricatorRequest, void>
    {
    public:
        MetricatorRequestReactor(sg::microreactor::Connection& connection, std::shared_ptr<metricatorapi::MetricatorRequest> message);
        virtual ~MetricatorRequestReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // metricator_MetricatorRequestReactor
