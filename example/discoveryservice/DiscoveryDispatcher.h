#pragma once
#ifndef sg_service_DiscoveryDispatcher
#define sg_service_DiscoveryDispatcher

#include "Dispatcher.h"


namespace sg { namespace service
{
    class DiscoveryDispatcher : public sg::microreactor::Dispatcher
    {
    public:
        explicit DiscoveryDispatcher();
        virtual ~DiscoveryDispatcher();

    public:
        virtual void Dispatch(sg::microreactor::Connection& connection) override;

    protected:
        std::stringstream mReceiveStream;
    };
}}

#endif // sg_service_DiscoveryDispatcher
