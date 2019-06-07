#pragma once
#ifndef sg_service_DiscoveryService
#define sg_service_DiscoveryService

#include "UdpSocket.h"
#include "RestService.h"
#include "DiscoveryDispatcher.h"
#include "ServiceDescription.h"


namespace sg { namespace service
{
    class DiscoveryService : public sg::microreactor::RestService
    {
    public:
        explicit DiscoveryService(const std::string& interfaceAddress = {}, const std::string& multicastAddress = DEFAULT_MULTICAST_ADDRESS, uint16_t multicastPort = DEFAULT_MULTICAST_PORT);
        virtual ~DiscoveryService();
        
        PROPERTY(Description, ServiceDescription);

    public:
        std::string GetMulticastAddress() { return mMulticastAddress; }
        uint16_t GetMulticastPort() { return mEndpoint->GetProfile()->Port.cref(); }

        virtual bool Initialize() override;

        virtual bool AdvertiseAlive();
        virtual bool AdvertiseByebye();

        virtual void RegisterDescriptionReactorFactory(const std::string& uri, sg::microreactor::RestDispatcher::Factory factory);

    protected:
        virtual std::shared_ptr<sg::microreactor::Reactor> CreateMSearchReactor(std::shared_ptr<sg::microreactor::RestMessage> message, sg::microreactor::Connection& connection);
    
    protected:
        std::string mInterfaceAddress;
        std::string mMulticastAddress;
        uint16_t mMulticastPort;
        std::shared_ptr<sg::microreactor::UdpSocket> mSocket;
        std::shared_ptr<sg::microreactor::RestService> mDescriptionService;
        std::string mDescriptionUri;
        sg::microreactor::RestDispatcher::Factory mDescriptionReactorFactory;
    };
}}

#endif // sg_service_DiscoveryService

