#pragma once
#ifndef metricator_MetricatorMessageDecoder
#define metricator_MetricatorMessageDecoder

#include "MessageDecoder.h"
#include "BinarySerializer.h"


namespace metricator
{
    class MetricatorMessageDecoder : public sg::microreactor::MessageDecoder
    {
    public:
        MetricatorMessageDecoder();
        virtual ~MetricatorMessageDecoder();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        sg::microreactor::BinarySerializer mSerializer;
    };
}

#endif // metricator_MetricatorMessageDecoder
