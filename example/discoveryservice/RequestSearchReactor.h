#pragma once
#ifndef sg_service_RequestSearchReactor
#define sg_service_RequestSearchReactor

#include "MessageReactor.h"
#include "RequestSearch.h"


namespace sg { namespace service
{
    class DiscoveryApiEncoder;

    class RequestSearchReactor : public sg::microreactor::MessageReactor<RequestSearch, DiscoveryApiEncoder>
    {
    public:
        RequestSearchReactor(sg::microreactor::Connection& connection, std::shared_ptr<RequestSearch> message);
        virtual ~RequestSearchReactor();

    public:
        virtual bool Process() override;
    };
}}


#endif // sg_service_RequestSearchReactor
