#pragma once
#ifndef metricatorapi_MetricatorRequest
#define metricatorapi_MetricatorRequest

#include <vector>
#include "Serializable.h"
#include "Message.h"
#include "Metric.h"


namespace metricatorapi
{
    class MetricatorRequest : public microreactor::Message
    {
    public:
        MetricatorRequest();
        virtual ~MetricatorRequest();

    public:
        const std::vector<Metric>& GetMetrics();
        void SetMetrics(const std::vector<Metric>& metrics);
        
        virtual bool Encode( std::ostream& stream) const override;
        virtual bool Decode(std::istream& stream) override;
        
    private:
        std::vector<Metric> mMetrics;
    };
}

#endif // metricatorapi_MetricatorRequest
