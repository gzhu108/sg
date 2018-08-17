#include "DiscoveryRequester.h"
#include "MSearchMessage.h"

using namespace sg::microreactor;
using namespace sg::service;


DiscoveryRequester::DiscoveryRequester(std::shared_ptr<Connection> connection)
    : MessageRequester(connection)
{
}

DiscoveryRequester::~DiscoveryRequester()
{
}

void DiscoveryRequester::MulticastSearch(const std::string& serviceType, const std::string& multicastAddress, uint16_t port)
{
#if 0
    std::shared_ptr<RequestSearch> request = std::make_shared<RequestSearch>();
    request->Endpoint.set(multicastAddress + ":" + std::to_string(port));
    request->Mx.set(5);
    request->St.set(serviceType);
    
    if (!SendMessage(request))
    {
        auto peerName = mConnection->GetPeerName();
        auto peerPort = mConnection->GetPeerPort();
        LOG("Failed to send multicast search request to %s:%u", peerName.c_str(), peerPort);
    }
#endif

    mConnection->SetPeerName(multicastAddress);
    mConnection->SetPeerPort(port);

    auto message = std::make_shared<MSearchMessage>();
    SendMessage(message);
}

void DiscoveryRequester::UnicastSearch(const std::string& serviceType, const std::string& unicastAddress, uint16_t port)
{
#if 0
    std::shared_ptr<RequestSearch> request = std::make_shared<RequestSearch>();
    request->Endpoint.set(unicastAddress + ":" + std::to_string(port));
    request->Mx.set(0);
    request->St.set(serviceType);

    if (!SendMessage(request))
    {
        auto peerName = mConnection->GetPeerName();
        auto peerPort = mConnection->GetPeerPort();
        LOG("Failed to send unicast search request to %s:%u", peerName.c_str(), peerPort);
    }
#endif
}
