#pragma once
#ifndef sg_service_DiscoveryDispatcher
#define sg_service_DiscoveryDispatcher

#include "RestDispatcher.h"


namespace sg { namespace service
{
    class DiscoveryDispatcher : public microreactor::RestDispatcher
    {
    protected:
        virtual void SendBadMessageResponse(microreactor::Connection& connection) override {}
        virtual void SendNotFoundResponse(microreactor::Connection& connection) override {}
    };
}}

#endif // sg_service_DiscoveryDispatcher

