#pragma once
#ifndef sg_service_DiscoveryClient
#define sg_service_DiscoveryClient

#include "UdpSocket.h"
#include "Client.h"
#include "DiscoveryDispatcher.h"
#include "ServiceDescription.h"


namespace sg { namespace service
{
    class DiscoveryClient : public microreactor::Client
    {
    public:
        explicit DiscoveryClient(std::shared_ptr<DiscoveryDispatcher> dispatcher, const std::string& interfaceAddress = {}, const std::string& multicastAddress = DEFAULT_MULTICAST_ADDRESS, uint16_t multicastPort = DEFAULT_MULTICAST_PORT);
        virtual ~DiscoveryClient();

        PROPERTY(ServiceType, std::string);

        microreactor::Signal<ServiceDescription>& ServiceFound = mServiceFound;
        microreactor::Signal<microreactor::Uuid>& Byebye = mByebye;

    public:
        std::string GetMulticastAddress() { return mMulticastAddress; }
        uint16_t GetMulticastPort() { return mMulticastPort; }

        void MulticastMSearch(const std::string& multicastAddress, uint16_t port, const std::string& mx);

    protected:
        virtual void Initialize(std::shared_ptr<microreactor::Connection> connection) override;
        
        virtual std::shared_ptr<microreactor::Reactor> CreateNotifyReactor(std::shared_ptr<microreactor::RestMessage> message, microreactor::Connection& connection);
        virtual std::shared_ptr<microreactor::Reactor> CreateMSearchResponseReactor(std::shared_ptr<microreactor::RestMessage> message, microreactor::Connection& connection);

    protected:
        microreactor::Emittable<ServiceDescription> mServiceFound;
        microreactor::Emittable<microreactor::Uuid> mByebye;
        std::string mInterfaceAddress;
        std::string mMulticastAddress;
        uint16_t mMulticastPort;
        std::shared_ptr<microreactor::UdpSocket> mSocket;
   };
}}

#endif // sg_service_DiscoveryClient
