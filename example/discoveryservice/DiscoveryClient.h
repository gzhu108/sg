#pragma once
#ifndef sg_service_DiscoveryClient
#define sg_service_DiscoveryClient

#include "UdpSocket.h"
#include "Client.h"
#include "DiscoveryDispatcher.h"
#include "ServiceDescription.h"


namespace sg { namespace service
{
    class DiscoveryClient : public sg::microreactor::Client
    {
    public:
        explicit DiscoveryClient(std::shared_ptr<DiscoveryDispatcher> dispatcher, const std::string& multicastAddress = DEFAULT_MULTICAST_ADDRESS, uint16_t port = DEFAULT_MULTICAST_PORT);
        virtual ~DiscoveryClient();

        PROPERTY(ServiceType, std::string);

        sg::microreactor::Signal<ServiceDescription>& ServiceFound = mServiceFound;
        sg::microreactor::Signal<sg::microreactor::Uuid>& Byebye = mByebye;

    public:
        std::string GetMulticastAddress() { return mMulticastAddress; }
        uint16_t GetMulticastPort() { return mPort; }

        void MulticastMSearch(const std::string& multicastAddress, uint16_t port, const std::string& mx);

    protected:
        virtual void Initialize(std::shared_ptr<sg::microreactor::Connection> connection, const std::chrono::milliseconds& timeout) override;
        
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateNotifyReactor(std::shared_ptr<sg::microreactor::RestMessage> message, std::shared_ptr<sg::microreactor::Connection> connection);
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateMSearchResponseReactor(std::shared_ptr<sg::microreactor::RestMessage> message, std::shared_ptr<sg::microreactor::Connection> connection);

    protected:
        sg::microreactor::Emittable<ServiceDescription> mServiceFound;
        sg::microreactor::Emittable<sg::microreactor::Uuid> mByebye;
        std::shared_ptr<sg::microreactor::UdpSocket> mSocket;
        std::string mMulticastAddress;
        uint16_t mPort;
   };
}}

#endif // sg_service_DiscoveryClient
