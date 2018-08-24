#pragma once
#ifndef sg_service_DiscoveryDispatcher
#define sg_service_DiscoveryDispatcher

#include "RestDispatcher.h"


namespace sg { namespace service
{
    class DiscoveryDispatcher : public sg::microreactor::RestDispatcher
    {
    protected:
        virtual void SendBadMessageResponse(sg::microreactor::Connection& connection) override {}
        virtual void SendNotFoundResponse(sg::microreactor::Connection& connection) override {}
    };
}}

#endif // sg_service_DiscoveryDispatcher

