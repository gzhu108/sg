#pragma once
#ifndef metricator_GetDescriptionReactor
#define metricator_GetDescriptionReactor

#include "Microreactor.h"


namespace metricator
{
    class GetDescriptionReactor : public microreactor::RestReactor<microreactor::RestRequest>
    {
    public:
        GetDescriptionReactor(microreactor::Connection& connection, std::shared_ptr<microreactor::RestRequest> request);
        virtual ~GetDescriptionReactor();

    public:
        virtual bool Process() override;
    };
}


#endif // metricator_GetDescriptionReactor
