#pragma once
#ifndef metricator_GetDescriptionReactor
#define metricator_GetDescriptionReactor

#include "Microreactor.h"


namespace metricator
{
    class GetDescriptionReactor : public sg::microreactor::RestReactor<sg::microreactor::RestRequest>
    {
    public:
        GetDescriptionReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~GetDescriptionReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // metricator_GetDescriptionReactor
