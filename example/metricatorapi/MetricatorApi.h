#pragma once
#ifndef metricatorapi_MetricatorApi
#define metricatorapi_MetricatorApi

#include <vector>
#include "Api.h"
#include "MetricatorRequestEncoder.h"
#include "Metric.h"


namespace metricatorapi
{
    class MetricatorApi : public sg::microreactor::Api<MetricatorRequestEncoder>
    {
    public:
        MetricatorApi(sg::microreactor::Connection& connection, std::shared_ptr<MetricatorRequestEncoder> metricatorRequestEncoder);
        virtual ~MetricatorApi();
        
    public:
        // API
        bool WriteMetrics(const std::vector<Metric>& metrics);
    };
}

#endif // metricatorapi_MetricatorApi
