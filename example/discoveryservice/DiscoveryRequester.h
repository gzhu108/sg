#pragma once
#ifndef sg_service_DiscoveryRequester
#define sg_service_DiscoveryRequester

#include "MessageRequester.h"
#include "RequestSearch.h"


namespace sg { namespace service
{
    class DiscoveryRequester : public sg::microreactor::MessageRequester
    {
    public:
        explicit DiscoveryRequester(std::shared_ptr<sg::microreactor::Connection> connection);
        virtual ~DiscoveryRequester();
        
    public:
        // API
        void MulticastSearch(const std::string& serviceType, const std::string& multicastAddress, uint16_t port);
        void UnicastSearch(const std::string& serviceType, const std::string& unicastAddress, uint16_t port);
	};
}}

#endif // sg_service_DiscoveryRequester
