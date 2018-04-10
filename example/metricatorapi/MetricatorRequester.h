#pragma once
#ifndef metricatorapi_MetricatorRequester
#define metricatorapi_MetricatorRequester

#include <vector>
#include "MessageRequester.h"
#include "Metric.h"


namespace metricatorapi
{
    class MetricatorRequester : public sg::microreactor::MessageRequester
    {
    public:
        MetricatorRequester(std::shared_ptr<sg::microreactor::Connection> connection);
        virtual ~MetricatorRequester();
        
    public:
        // API
        bool WriteMetrics(const std::vector<Metric>& metrics);
    };
}

#endif // metricatorapi_MetricatorRequester
