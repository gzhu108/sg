#pragma once
#ifndef metricator_MetricatorMessageDispatcher
#define metricator_MetricatorMessageDispatcher

#include "MessageDispatcher.h"
#include "BinarySerializer.h"


namespace metricator
{
    class MetricatorMessageDispatcher : public sg::microreactor::MessageDispatcher
    {
    public:
        MetricatorMessageDispatcher();
        virtual ~MetricatorMessageDispatcher();

    public:
        virtual std::shared_ptr<sg::microreactor::Reactor> Decode(std::istream& stream, sg::microreactor::Connection& connection) override;

    protected:
        sg::microreactor::BinarySerializer mSerializer;
    };
}

#endif // metricator_MetricatorMessageDispatcher
