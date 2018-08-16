#pragma once
#ifndef sg_service_MSearchReactor
#define sg_service_MSearchReactor

#include "RestReactor.h"
#include "MSearchMessage.h"


namespace sg { namespace service
{
    class MSearchReactor : public sg::microreactor::RestReactor<MSearchMessage>
    {
    public:
        MSearchReactor(std::shared_ptr<sg::microreactor::Connection> connection, std::shared_ptr<sg::microreactor::RestRequest> request);
        virtual ~MSearchReactor();

    public:
        virtual bool Process() override;
    };
}}


#endif // sg_service_MSearchReactor
