#include "MetricatorLogger.h"
#include "NetworkUtility.h"
#include "MetricatorRequester.h"

using namespace sg::microreactor;
using namespace metricatorapi;


MetricatorLogger::MetricatorLogger(const std::string& protocol, const std::string& address, uint16_t port)
{
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set(protocol);
    profile->Address.set(address);
    profile->Port.set(port);

    mConnection = NetworkUtility::CreateConnection(profile);
}

MetricatorLogger::~MetricatorLogger()
{
}

void MetricatorLogger::Log(const std::string& text)
{
    if (mConnection == nullptr)
    {
        return;
    }

    std::vector<Metric> metrics;

    Metric log;
    log.mKey = "LOG";
    log.mValue = text;
    metrics.emplace_back(log);

    MetricatorRequester requester(mConnection);
    requester.WriteMetrics(metrics);
}
