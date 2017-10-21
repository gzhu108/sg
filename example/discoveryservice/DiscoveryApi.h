#pragma once
#ifndef sg_service_DiscoveryApi
#define sg_service_DiscoveryApi

#include "Api.h"
#include "RequestSearch.h"


namespace sg { namespace service
{
    class DiscoveryApiEncoder
    {
    public:
        bool EncodeMessage(std::shared_ptr<RequestSearch> message, std::ostream& stream)
        {
            return true;
        }
    };

    class DiscoveryApi : public sg::microreactor::Api<DiscoveryApiEncoder>
    {
    public:
        explicit DiscoveryApi(sg::microreactor::Connection& connection);
        virtual ~DiscoveryApi();
        
    public:
        // API
        void MulticastSearch(const std::string& serviceType, const std::string& multicastAddress, uint16_t port);
        void UnicastSearch(const std::string& serviceType, const std::string& unicastAddress, uint16_t port);
	};
}}

#endif // sg_service_DiscoveryApi
