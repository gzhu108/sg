#include "MetricatorMessageDecoder.h"
#include "MetricatorRequest.h"
#include "MetricatorRequestReactor.h"

using namespace sg::microreactor;
using namespace metricatorapi;
using namespace metricator;


MetricatorMessageDecoder::MetricatorMessageDecoder()
{
}

MetricatorMessageDecoder::~MetricatorMessageDecoder()
{
}

std::shared_ptr<sg::microreactor::Reactor> MetricatorMessageDecoder::Decode(std::istream& stream, Connection& connection)
{
    if (GetStreamSize(stream) == 0)
    {
        return nullptr;
    }

    auto message = std::make_shared<MetricatorRequest>();
    if (!message->Decode(stream))
    {
        return nullptr;
    }

    return std::make_shared<MetricatorRequestReactor>(connection, message);
}
