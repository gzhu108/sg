#pragma once
#ifndef metricator_MetricatorMessageDispatcher
#define metricator_MetricatorMessageDispatcher

#include "MessageDispatcher.h"
#include "BinarySerializer.h"


namespace metricator
{
    class MetricatorMessageDispatcher : public microreactor::MessageDispatcher
    {
    public:
        MetricatorMessageDispatcher();
        virtual ~MetricatorMessageDispatcher();

    public:
        virtual std::shared_ptr<microreactor::Reactor> Decode(std::istream& stream, microreactor::Connection& connection) override;

    protected:
        microreactor::BinarySerializer mSerializer;
    };
}

#endif // metricator_MetricatorMessageDispatcher
