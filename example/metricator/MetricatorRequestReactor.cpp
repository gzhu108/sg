#include "MetricatorRequestReactor.h"

using namespace sg::microreactor;
using namespace metricatorapi;
using namespace metricator;


MetricatorRequestReactor::MetricatorRequestReactor(std::shared_ptr<Connection> connection, std::shared_ptr<MetricatorRequest> message)
    : MessageReactor(connection, message)
{
}

MetricatorRequestReactor::~MetricatorRequestReactor()
{
}

bool MetricatorRequestReactor::Process()
{
    std::stringstream s;
    const auto& metrics = InputMessage()->GetMetrics();
    for (const auto& metric : metrics)
    {
        s << metric.mKey << ": " << metric.mValue;
    }

    //LOG("%s: %s", mConnection.Name->c_str(), s.str().c_str());
    printf("%s: %s\n", mConnection->Name->c_str(), s.str().c_str());

    return true;
}
