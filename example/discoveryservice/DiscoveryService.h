#pragma once
#ifndef sg_service_DiscoveryService
#define sg_service_DiscoveryService

#include "UdpSocket.h"
#include "RestService.h"
#include "DiscoveryDispatcher.h"
#include "ServiceDescription.h"


namespace sg { namespace service
{
    const std::string DEFAULT_MULTICAST_ADDRESS("239.255.255.250");
    const uint16_t DEFAULT_MULTICAST_PORT = 1900;

    class DiscoveryService : public sg::microreactor::RestService
    {
    public:
        explicit DiscoveryService(const std::string& serverAddress, uint16_t port = DEFAULT_MULTICAST_PORT, const std::string& multicastAddress = DEFAULT_MULTICAST_ADDRESS);
        virtual ~DiscoveryService();
        
        PROPERTY(Description, ServiceDescription);

    public:
        std::string GetMulticastAddress() { return mMulticastAddress; }
        uint16_t GetMulticastPort() { return mProfile->Port.cref(); }

        virtual bool Initialize() override;

        virtual bool AdvertiseAlive();
        virtual bool AdvertiseByebye();

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateMSearchReactor(std::shared_ptr<sg::microreactor::RestMessage> message, std::shared_ptr<sg::microreactor::Connection> connection);
        
    protected:
        std::shared_ptr<sg::microreactor::UdpSocket> mSocket;
        std::string mMulticastAddress;
    };
}}

#endif // sg_service_DiscoveryService

