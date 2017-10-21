#pragma once
#ifndef metricatorapi_MetricatorRequestEncoder
#define metricatorapi_MetricatorRequestEncoder

#include "BinarySerializer.h"
#include "MetricatorRequest.h"


namespace metricatorapi
{
    class MetricatorRequestEncoder
    {
    public:
        MetricatorRequestEncoder();
        virtual ~MetricatorRequestEncoder();

    public:
        virtual bool EncodeMessage(std::shared_ptr<MetricatorRequest> message, std::ostream& stream);

    protected:
        sg::microreactor::BinarySerializer mSerializer;
    };
}

#endif // metricatorapi_MetricatorRequestEncoder
