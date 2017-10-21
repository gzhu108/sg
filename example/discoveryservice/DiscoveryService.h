#pragma once
#ifndef sg_service_DiscoveryService
#define sg_service_DiscoveryService

#include "UdpSocket.h"
#include "Client.h"
#include "Microservice.h"
#include "RequestSearch.h"


namespace sg { namespace service
{
    const std::string DEFAULT_MULTICAST_ADDRESS("239.255.255.250");
    const uint16_t DEFAULT_MULTICAST_PORT = 1900;

    class DiscoveryService : public sg::microreactor::Microservice
    {
    public:
        explicit DiscoveryService(std::shared_ptr<sg::microreactor::UdpSocket> socket, const std::string& multicastAddress = DEFAULT_MULTICAST_ADDRESS, uint16_t port = DEFAULT_MULTICAST_PORT);
        virtual ~DiscoveryService();
        
    public:
        std::shared_ptr<sg::microreactor::UdpSocket> GetSocket() { return mSocket; }
        std::string GetMulticastAddress() { return mMulticastAddress; }
        uint16_t GetMulticastPort() { return mMulticastPort; }

    protected:
        // Microservice
        virtual bool Initialize() override;
        
    protected:
        std::shared_ptr<sg::microreactor::UdpSocket> mSocket;
        std::string mMulticastAddress;
        uint16_t mMulticastPort;
    };
}}

#endif // sg_service_DiscoveryService

