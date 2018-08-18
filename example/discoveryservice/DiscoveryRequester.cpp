#include "DiscoveryRequester.h"
#include "RestRequest.h"

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
    mConnection->SetPeerName(multicastAddress);
    mConnection->SetPeerPort(port);

    RestRequest request;
    request.mMethod = "M-SEARCH";
    request.mUri = "*";
    request.mVersion = "HTTP/1.1";
    request.mHeaders.emplace_back(HttpHeader("HOST", multicastAddress + ":" + std::to_string(port)));
    request.mHeaders.emplace_back(HttpHeader("MAN", "ssdp:discover"));
    request.mHeaders.emplace_back(HttpHeader("MX", "2"));
    request.mHeaders.emplace_back(HttpHeader("ST", serviceType));

    std::string buffer;
    if (request.FlushToBuffer(buffer))
    {
        mConnection->Send(buffer.data(), buffer.length());
    }
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
