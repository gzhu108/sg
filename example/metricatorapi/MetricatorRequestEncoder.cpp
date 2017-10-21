#include "MetricatorRequestEncoder.h"

using namespace sg::microreactor;
using namespace metricatorapi;


MetricatorRequestEncoder::MetricatorRequestEncoder()
{
}

MetricatorRequestEncoder::~MetricatorRequestEncoder()
{
}

bool MetricatorRequestEncoder::EncodeMessage(std::shared_ptr<MetricatorRequest> message, std::ostream& stream)
{
    return message->Serialize(mSerializer, stream);
}
