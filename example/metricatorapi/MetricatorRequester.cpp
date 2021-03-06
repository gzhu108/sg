#include "MetricatorRequester.h"
#include "MetricatorRequest.h"

using namespace microreactor;
using namespace metricatorapi;


MetricatorRequester::MetricatorRequester(Connection& connection)
    : MessageRequester(connection)
{
}

MetricatorRequester::~MetricatorRequester()
{
}

bool MetricatorRequester::WriteMetrics(const std::vector<Metric>& metrics)
{
    std::shared_ptr<MetricatorRequest> request = std::make_shared<MetricatorRequest>();
    request->SetMetrics(metrics);
    return SendMessage(request, nullptr);
}
