#pragma once
#ifndef sg_service_DiscoveryClient
#define sg_service_DiscoveryClient

#include "UdpSocket.h"
#include "Client.h"
#include "RestService.h"
#include "RestReactor.h"


namespace sg { namespace service
{
    const std::string DEFAULT_MULTICAST_ADDRESS("239.255.255.250");
    const uint16_t DEFAULT_MULTICAST_PORT = 1900;

    class DiscoveryClient : public sg::microreactor::Client
    {
    public:
        DiscoveryClient(const std::string& multicastAddress = DEFAULT_MULTICAST_ADDRESS, uint16_t port = DEFAULT_MULTICAST_PORT);
        virtual ~DiscoveryClient();

    public:
        std::string GetMulticastAddress() { return mMulticastAddress; }
        uint16_t GetMulticastPort() { return mPort; }

        void MulticastMSearch(const std::string& serviceType, const std::string& multicastAddress, uint16_t port);
        void UnicastMSearch(const std::string& serviceType, const std::string& unicastAddress, uint16_t port);

        void RegisterResponseReactorFactory(sg::microreactor::RestDispatcher::Factory factory);

    protected:
        virtual void Initialize(std::shared_ptr<sg::microreactor::Connection> connection, const std::chrono::milliseconds& timeout) override;
        void SendMSearch(const std::string& serviceType, const std::string& address, uint16_t port, const std::string& mx);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateNotifyReactor(std::shared_ptr<sg::microreactor::RestMessage> message, std::shared_ptr<sg::microreactor::Connection> connection);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateResponseReactor(std::shared_ptr<sg::microreactor::RestMessage> message, std::shared_ptr<sg::microreactor::Connection> connection);

    protected:
        std::shared_ptr<sg::microreactor::UdpSocket> mSocket;
        std::string mMulticastAddress;
        uint16_t mPort;
   };
}}

#endif // sg_service_DiscoveryClient
