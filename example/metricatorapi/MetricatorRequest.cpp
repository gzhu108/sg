#include "MetricatorRequest.h"
#include "BinarySerializer.h"

using namespace microreactor;
using namespace metricatorapi;


MetricatorRequest::MetricatorRequest()
{
}

MetricatorRequest::~MetricatorRequest()
{
}

const std::vector<Metric>& MetricatorRequest::GetMetrics()
{
    return mMetrics;
}

void MetricatorRequest::SetMetrics(const std::vector<Metric>& metrics)
{
    mMetrics = std::move(metrics);
}

bool MetricatorRequest::Encode(std::ostream& stream) const
{
    microreactor::BinarySerializer serializer;

    uint64_t count = mMetrics.size();
    serializer.Write(count, stream);
    
    for (const Metric& metric : mMetrics)
    {
        serializer.Write(metric.mKey, stream);
        serializer.Write(metric.mValue, stream);
    }
 
    return true;
}

bool MetricatorRequest::Decode(std::istream& stream)
{
    microreactor::BinarySerializer serializer;

    uint64_t count = 0;
    if (!serializer.Read(stream, count))
    {
        return false;
    }
    
    for (uint64_t i = 0; i < count; ++i)
    {
        Metric metric;

        if (!serializer.Read(stream, metric.mKey))
        {
            return false;
        }

        if (!serializer.Read(stream, metric.mValue))
        {
            return false;
        }

        mMetrics.emplace_back(metric);
    }

    return true;
}
