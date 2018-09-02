#include "DiscoveryService.h"
#include "NetworkUtility.h"
#include "TcpEndpoint.h"
#include "UdpEndpoint.h"
#include "MSearchReactor.h"

using namespace sg::microreactor;
using namespace sg::service;


DiscoveryService::DiscoveryService(const std::string& interfaceAddress, const std::string& multicastAddress, uint16_t port)
    : mInterfaceAddress(interfaceAddress)
    , mMulticastAddress(multicastAddress)
{
    mRestDispatcher = std::make_shared<DiscoveryDispatcher>();

    std::string address = mInterfaceAddress;
    if (address.empty())
    {
        address = "0.0.0.0";
        std::shared_ptr<addrinfo> addrInfo = NetworkUtility::GetAddressInfo(multicastAddress, port, SOCK_DGRAM, IPPROTO_UDP, false);
        if (addrInfo != nullptr)
        {
            if (addrInfo->ai_addr->sa_family == AF_INET6)
            {
                address = "::";
            }
        }
    }

    // Create service profile
    mProfile = std::make_shared<Profile>();
    mProfile->Protocol.set("udp");
    mProfile->Address.set(address);
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
        AdvertiseByebye();
        mSocket->LeaveMulticastGroup(mMulticastAddress, mInterfaceAddress);
    }
}

bool DiscoveryService::Initialize()
{
    // Create UDP socket
    mSocket = std::make_shared<UdpSocket>();

    // Create UDP endpoint
    mEndpoint = std::make_shared<UdpEndpoint>(mSocket, mProfile);
    if (!mSocket->IsValid() || mDescriptionReactorFactory == nullptr)
    {
        return false;
    }

    mEndpoint->ListenTimeout.set(std::chrono::milliseconds(30));
    mEndpoint->ReceiveTimeout.set(std::chrono::milliseconds(30));
    mEndpoint->SendTimeout.set(std::chrono::milliseconds(1000));

    try
    {
        // Initialize multicasting
        if (mSocket->JoinMulticastGoup(mMulticastAddress, mInterfaceAddress, false))
        {
            if (RestService::Initialize())
            {
                LOG("Discovery server endpoint: [%s]:%d on interface %s", mSocket->HostName->c_str(), mSocket->HostPort.cref(), mInterfaceAddress.c_str());
                
                std::string location = "http://";
                location += mInterfaceAddress + ":" + std::to_string(mSocket->HostPort.cref()) + mDescriptionUri;
                Description->Location.set(location);

                auto descriptionServiceDispatcher = std::make_shared<RestDispatcher>();
                descriptionServiceDispatcher->RegisterRestReactorFactory("GET", mDescriptionUri, mDescriptionReactorFactory);

                auto descriptionServiceProfile = std::make_shared<Profile>();
                descriptionServiceProfile->Protocol.set("tcp");
                descriptionServiceProfile->Address.set(mInterfaceAddress);
                descriptionServiceProfile->Port.set(mSocket->HostPort.cref());
                descriptionServiceProfile->Dispatcher.set(descriptionServiceDispatcher);

                mDescriptionService = std::make_shared<RestService>(descriptionServiceProfile);
                return mDescriptionService->Start();
            }
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
    request.mHeaders.emplace_back(HttpHeader("USN", Description->Usn.cref()));
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
    request.mHeaders.emplace_back(HttpHeader("USN", Description->Usn.cref()));
    request.mHeaders.emplace_back(HttpHeader("NT", Description->ServiceType.cref()));

    std::string buffer;
    if (request.FlushToBuffer(buffer))
    {
        int32_t bytesSent = 0;
        return mSocket->SendTo(buffer.data(), buffer.length(), mMulticastAddress, mProfile->Port.cref(), bytesSent);
    }

    return false;
}

void DiscoveryService::RegisterDescriptionReactorFactory(const std::string& uri, sg::microreactor::RestDispatcher::Factory factory)
{
    mDescriptionUri = uri;
    mDescriptionReactorFactory = factory;
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
        reactor->Description.set(Description.cref());
    }

    return reactor;
}
