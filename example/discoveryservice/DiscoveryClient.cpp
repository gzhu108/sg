#include "DiscoveryClient.h"
#include "UdpEndpoint.h"
#include "RestDispatcher.h"
#include "NotifyReactor.h"

using namespace sg::microreactor;
using namespace sg::service;


DiscoveryClient::DiscoveryClient(const std::string& multicastAddress, uint16_t port)
    : mMulticastAddress(multicastAddress)
{
    auto dispatcher = std::make_shared<RestDispatcher>();

    // Create client profile
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set("udp");
    profile->Address.set("127.0.0.1");
    profile->Port.set(port);
    profile->Dispatcher.set(dispatcher);

    // Register M-SEARCH
    if (mRestDispatcher)
    {
        mRestDispatcher->ReplyError.set(false);
        mRestDispatcher->RegisterRestReactorFactory("NOTIFY", "*", std::bind(&DiscoveryClient::CreateNotifyReactor, this, std::placeholders::_1, std::placeholders::_2));
    }
}

DiscoveryClient::~DiscoveryClient()
{
}

bool DiscoveryClient::Initialize()
{
    // Create UDP socket
    mSocket = std::make_shared<UdpSocket>();

    // Create UDP endpoint
    mEndpoint = std::make_shared<UdpEndpoint>(mSocket, mProfile);
    if (!mSocket->IsValid())
    {
        return false;
    }

    mEndpoint->ListenTimeout.set(std::chrono::milliseconds(30));
    mEndpoint->ReceiveTimeout.set(std::chrono::milliseconds(30));
    mEndpoint->SendTimeout.set(std::chrono::milliseconds(1000));

    try
    {
        LOG("Discovery Server Endpoint: [%s]:%d", mSocket->HostName->c_str(), mSocket->HostPort.cref());

        uint32_t multicastInterfaceIndex = 0;
        std::vector<NetworkUtility::NetworkInterfaceInfo> networkInterfaceInfoList;
        if (NetworkUtility::GetNetworkInterfaceInfo(networkInterfaceInfoList))
        {
            for (auto& networkInterface : networkInterfaceInfoList)
            {
                if (!networkInterface.mAddress.empty() && networkInterface.mAddress != "0.0.0.0")
                {
                    //multicastInterfaceIndex = networkInterface.mIndex;
                    break;
                }
            }
        }

        // Initialize multicasting
        if (mSocket->JoinMulticastGoup(mMulticastAddress, multicastInterfaceIndex))
        {
            return RestService::Initialize();
        }
    }
    catch (SocketException& e)
    {
        LOG("Error: [%s]:[%u] %s (%d)", e.mName.c_str(), e.mPort, e.what(), e.mError);
    }

    return false;
}

void DiscoveryClient::MulticastMSearch(const std::string& serviceType, const std::string& multicastAddress, uint16_t port)
{
    mConnection->SetPeerName(multicastAddress);
    mConnection->SetPeerPort(port);
    SendMSearch(serviceType, multicastAddress, port, "2");
}

void DiscoveryClient::UnicastMSearch(const std::string& serviceType, const std::string& unicastAddress, uint16_t port)
{
    mConnection->SetPeerName(unicastAddress);
    mConnection->SetPeerPort(port);
    SendMSearch(serviceType, unicastAddress, port, "0");
}

void DiscoveryClient::SendMSearch(const std::string& serviceType, const std::string& address, uint16_t port, const std::string& mx)
{
    RestRequest request;
    request.mMethod = "M-SEARCH";
    request.mUri = "*";
    request.mVersion = "HTTP/1.1";
    request.mHeaders.emplace_back(HttpHeader("HOST", address + ":" + std::to_string(port)));
    request.mHeaders.emplace_back(HttpHeader("MAN", "ssdp:discover"));
    request.mHeaders.emplace_back(HttpHeader("MX", mx));
    request.mHeaders.emplace_back(HttpHeader("ST", serviceType));

    std::string buffer;
    if (request.FlushToBuffer(buffer))
    {
        mConnection->Send(buffer.data(), buffer.length());
    }
}

std::shared_ptr<Reactor> DiscoveryClient::CreateNotifyReactor(std::shared_ptr<RestMessage> message, std::shared_ptr<Connection> connection)
{
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request->mUri.length() < std::string("*").length())
    {
        return nullptr;
    }

    auto reactor = std::make_shared<NotifyReactor>(connection, request);
    return reactor;
}
