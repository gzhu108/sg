#pragma once
#ifndef sg_service_DiscoveryClient
#define sg_service_DiscoveryClient

#include "UdpSocket.h"
#include "Client.h"
#include "RestService.h"


namespace sg { namespace service
{
    const std::string DEFAULT_MULTICAST_ADDRESS("239.255.255.250");
    const uint16_t DEFAULT_MULTICAST_PORT = 1900;

    class DiscoveryClient
        : public sg::microreactor::Client
        , public sg::microreactor::RestService
    {
    public:
        DiscoveryClient(const std::string& multicastAddress = DEFAULT_MULTICAST_ADDRESS, uint16_t port = DEFAULT_MULTICAST_PORT);
        virtual ~DiscoveryClient();

    public:
        std::string GetMulticastAddress() { return mMulticastAddress; }
        uint16_t GetMulticastPort() { return mProfile->Port.cref(); }

        virtual bool Initialize() override;

        void MulticastMSearch(const std::string& serviceType, const std::string& multicastAddress, uint16_t port);
        void UnicastMSearch(const std::string& serviceType, const std::string& unicastAddress, uint16_t port);

    protected:
        void SendMSearch(const std::string& serviceType, const std::string& address, uint16_t port, const std::string& mx);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateNotifyReactor(std::shared_ptr<sg::microreactor::RestMessage> message, std::shared_ptr<sg::microreactor::Connection> connection);

    protected:
        std::shared_ptr<sg::microreactor::UdpSocket> mSocket;
        std::string mMulticastAddress;
   };
}}

#endif // sg_service_DiscoveryClient
