#include "MetricatorApi.h"
#include "MetricatorRequest.h"

using namespace sg::microreactor;
using namespace metricatorapi;


MetricatorApi::MetricatorApi(Connection& connection, std::shared_ptr<MetricatorRequestEncoder> metricatorRequestEncoder)
    : Api(connection, metricatorRequestEncoder)
{
}

MetricatorApi::~MetricatorApi()
{
}

bool MetricatorApi::WriteMetrics(const std::vector<Metric>& metrics)
{
    std::shared_ptr<MetricatorRequest> request = std::make_shared<MetricatorRequest>();
    request->SetMetrics(metrics);
    return SendMessage(request);
}
