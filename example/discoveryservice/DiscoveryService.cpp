#include "DiscoveryService.h"
#include "NetworkUtility.h"
#include "UdpEndpoint.h"
#include "MSearchReactor.h"

using namespace sg::microreactor;
using namespace sg::service;


DiscoveryService::DiscoveryService(const std::string& multicastAddress, uint16_t port)
    : mMulticastAddress(multicastAddress)
{
    mRestDispatcher = std::make_shared<DiscoveryDispatcher>();

    // Create service profile
    mProfile = std::make_shared<Profile>();
    mProfile->Protocol.set("udp");
    mProfile->Address.set("0.0.0.0");   // TODO: depends on multicastAddress is ipv4 or ipv6
    mProfile->Port.set(port);
    mProfile->Dispatcher.set(mRestDispatcher);
    
    // Register M-SEARCH
    if (mRestDispatcher)
    {
        mRestDispatcher->RegisterRestReactorFactory("M-SEARCH", "*", std::bind(&DiscoveryService::CreateMSearchReactor, this, std::placeholders::_1, std::placeholders::_2));
    }
}

DiscoveryService::~DiscoveryService()
{
    if (mSocket != nullptr && mSocket->IsValid())
    {
        mSocket->LeaveMulticastGroup(mMulticastAddress, 0);
    }
}

bool DiscoveryService::Initialize()
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
        LOG("Discovery server endpoint: [%s]:%d", mSocket->HostName->c_str(), mSocket->HostPort.cref());

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
        if (mSocket->JoinMulticastGoup(mMulticastAddress, multicastInterfaceIndex, false))
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

bool DiscoveryService::AdvertiseAlive()
{
    RestRequest request;
    request.mMethod = "NOTIFY";
    request.mUri = "*";
    request.mVersion = "HTTP/1.1";
    request.mHeaders.emplace_back(HttpHeader("HOST", mMulticastAddress + ":" + std::to_string(mProfile->Port.cref())));
    request.mHeaders.emplace_back(HttpHeader("CACHE-CONTROL", std::string("max-age = ") + std::to_string(Description->NotifyMaxAge.cref())));
    request.mHeaders.emplace_back(HttpHeader("LOCATION", Description->Location.cref()));
    request.mHeaders.emplace_back(HttpHeader("NTS", "ssdp:alive"));
    request.mHeaders.emplace_back(HttpHeader("SERVER", Description->ServerInfo.cref()));
    request.mHeaders.emplace_back(HttpHeader("USN", Description->Usn->ToString()));
    request.mHeaders.emplace_back(HttpHeader("NT", Description->ServiceType.cref()));
    
    std::string buffer;
    if (request.FlushToBuffer(buffer))
    {
        int32_t bytesSent = 0;
        return mSocket->SendTo(buffer.data(), buffer.length(), mMulticastAddress, mProfile->Port.cref(), bytesSent);
    }

    return false;
}

bool DiscoveryService::AdvertiseByebye()
{
    RestRequest request;
    request.mMethod = "NOTIFY";
    request.mUri = "*";
    request.mVersion = "HTTP/1.1";
    request.mHeaders.emplace_back(HttpHeader("HOST", mMulticastAddress + ":" + std::to_string(mProfile->Port.cref())));
    request.mHeaders.emplace_back(HttpHeader("NTS", "ssdp:byebye"));
    request.mHeaders.emplace_back(HttpHeader("USN", Description->Usn->ToString()));
    request.mHeaders.emplace_back(HttpHeader("NT", Description->ServiceType.cref()));

    std::string buffer;
    if (request.FlushToBuffer(buffer))
    {
        int32_t bytesSent = 0;
        return mSocket->SendTo(buffer.data(), buffer.length(), mMulticastAddress, mProfile->Port.cref(), bytesSent);
    }

    return false;
}

std::shared_ptr<Reactor> DiscoveryService::CreateMSearchReactor(std::shared_ptr<RestMessage> message, std::shared_ptr<Connection> connection)
{
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request->mUri.length() < std::string("*").length())
    {
        return nullptr;
    }

    auto reactor = std::make_shared<MSearchReactor>(connection, request);
    if (reactor != nullptr)
    {
        reactor->Description->NotifyMaxAge.set(Description->NotifyMaxAge.cref());
        reactor->Description->Location.set(Description->Location.cref());
        reactor->Description->ServerInfo.set(Description->ServerInfo.cref());
        reactor->Description->Usn.set(Description->Usn.cref());
        reactor->Description->ServiceType.set(Description->ServiceType.cref());
    }

    return reactor;
}
