#include "DiscoveryClient.h"
#include "UdpEndpoint.h"
#include "DiscoveryDispatcher.h"
#include "NotifyReactor.h"
#include "MSearchResponseReactor.h"

using namespace sg::microreactor;
using namespace sg::service;


DiscoveryClient::DiscoveryClient(std::shared_ptr<DiscoveryDispatcher> dispatcher, const std::string& multicastAddress, uint16_t port)
    : mMulticastAddress(multicastAddress)
    , mPort(port)
{
    if (dispatcher == nullptr)
    {
        dispatcher = std::make_shared<DiscoveryDispatcher>();
    }

    // Create client profile
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set("udp");
    profile->Address.set("127.0.0.1");
    profile->Port.set(mPort);
    profile->Dispatcher.set(dispatcher);

    // Create UDP socket
    mSocket = std::make_shared<UdpSocket>();

    // Create UDP connection
    auto connection = std::make_shared<UdpConnection>(mSocket, profile);
    Initialize(connection, std::chrono::milliseconds(30));

    dispatcher->RegisterRestReactorFactory("NOTIFY", "*", std::bind(&DiscoveryClient::CreateNotifyReactor, this, std::placeholders::_1, std::placeholders::_2));
    dispatcher->RegisterRestReactorFactory("", "", std::bind(&DiscoveryClient::CreateMSearchResponseReactor, this, std::placeholders::_1, std::placeholders::_2));
}

DiscoveryClient::~DiscoveryClient()
{
}

void DiscoveryClient::MulticastMSearch(const std::string& multicastAddress, uint16_t port, const std::string& mx)
{
    mConnection->SetPeerName(multicastAddress);
    mConnection->SetPeerPort(port);

    RestRequest request;
    request.mMethod = "M-SEARCH";
    request.mUri = "*";
    request.mVersion = "HTTP/1.1";
    request.mHeaders.emplace_back(HttpHeader("HOST", multicastAddress + ":" + std::to_string(port)));
    request.mHeaders.emplace_back(HttpHeader("MAN", "ssdp:discover"));
    request.mHeaders.emplace_back(HttpHeader("MX", mx));
    request.mHeaders.emplace_back(HttpHeader("ST", ServiceType.cref()));

    std::string buffer;
    if (request.FlushToBuffer(buffer))
    {
        mConnection->Send(buffer.data(), buffer.length());
    }
}

void DiscoveryClient::Initialize(std::shared_ptr<Connection> connection, const std::chrono::milliseconds& timeout)
{
    try
    {
        LOG("Discovery client connection: [%s]:%d", mSocket->HostName->c_str(), mSocket->HostPort.cref());

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
            Client::Initialize(connection, timeout);
        }
    }
    catch (SocketException& e)
    {
        LOG("Error: [%s]:[%u] %s (%d)", e.mName.c_str(), e.mPort, e.what(), e.mError);
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
    reactor->ServiceType.set(ServiceType.cref());
    reactor->Byebye.Connect([&](const Uuid& usn)
    {
        mByebye(usn);
    });

    return reactor;
}

std::shared_ptr<Reactor> DiscoveryClient::CreateMSearchResponseReactor(std::shared_ptr<RestMessage> message, std::shared_ptr<Connection> connection)
{
    auto response = std::static_pointer_cast<RestResponse>(message);
    auto reactor = std::make_shared<MSearchResponseReactor>(connection, response);
    reactor->ServiceType.set(ServiceType.cref());
    reactor->ServiceFound.Connect([&](const ServiceDescription& description)
    {
        mServiceFound(description);
    });

    return reactor;
}
