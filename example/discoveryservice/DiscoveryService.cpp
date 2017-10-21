#include "DiscoveryService.h"
#include "Exception.h"
#include "DiscoveryDispatcher.h"
#include "DiscoveryApi.h"
#include "TaskManager.h"
#include "Connection.h"
#include "Listener.h"
#include "UdpHost.h"
#include "UdpConnection.h"

using namespace sg::microreactor;
using namespace sg::service;


DiscoveryService::DiscoveryService(std::shared_ptr<UdpSocket> socket, const std::string& multicastAddress, uint16_t port)
    : mSocket(socket)
    , mMulticastAddress(multicastAddress)
    , mMulticastPort(port)
{
    // Create service profile
    mProfile = std::make_shared<Profile>();
    mProfile->Protocol.set("udp");
    mProfile->Address.set(mMulticastAddress);
    mProfile->Port.set(mMulticastPort);
    mProfile->Dispatcher.set(std::make_shared<DiscoveryDispatcher>());
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
    LOG("Discovery Server HOST: [%s]:%d", mSocket->HostName->c_str(), mSocket->HostPort.cref());
   
    try
    {
        // Initialize multicasting
        if (mSocket->JoinMulticastGoup(mMulticastAddress, 0))
        {
            std::shared_ptr<Host> host = std::make_shared<UdpHost>(mSocket, mProfile);
            host->ListenTimeout.set(std::chrono::milliseconds(30));
            host->ReceiveTimeout.set(std::chrono::milliseconds(30));
            host->SendTimeout.set(std::chrono::milliseconds(1000));

            mListener = std::make_shared<Listener>();
            return mListener->Initialize(host);
        }
    }
    catch (SocketException& e)
    {
        LOG("Error: [%s]:[%u] %s (%d)", e.mName.c_str(), e.mPort, e.what(), e.mError);
    }

    return false;
}
