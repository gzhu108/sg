#include "UdpConnection.h"
#include <algorithm>
#include "Exception.h"
#include "UdpSocket.h"

using namespace sg::microreactor;


UdpConnection::UdpConnection(std::shared_ptr<UdpSocket> socket, std::shared_ptr<Profile> profile)
    : Connection(profile)
    , mSocket(socket)
    , mServerConnection(false)
    , mDataRetrieved(false)
{
    if (mSocket != nullptr && mSocket->IsValid())
    {
        // Server connection received from a client
        mServerConnection = true;
    }
    else
    {
        if (mSocket == nullptr)
        {
            mSocket = std::make_shared<UdpSocket>();
        }

        std::string address = LOCAL_HOST;
        std::shared_ptr<addrinfo> addrInfo = NetworkUtility::GetAddressInfo(mProfile->Address.cref(), mProfile->Port.cref(), SOCK_DGRAM, IPPROTO_UDP, false);
        if (addrInfo != nullptr && addrInfo->ai_addr->sa_family == AF_INET6)
        {
            address = LOCAL_HOST_IPV6;
        }

        try
        {
            mSocket->Bind(address, 0);
        }
        catch (SocketException& e)
        {
            // Socket exception received.
            LOG("Failed to bind: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
            Close();
        }

        mSocket->PeerAddress.set(mProfile->Address.cref());
        mSocket->PeerPort.set(mProfile->Port.cref());

        // Client connection to a server
        mServerConnection = false;
    }

    if (!mSocket->HostAddress->empty())
    {
        Name.set(std::string("[") + mSocket->HostAddress.cref() + "]:" + std::to_string(mSocket->HostPort.cref()));
    }

    mReceiveBufferSize = DEFAULT_UDP_CONNECTION_BUFFER_SIZE;
    mSocket->SetReceiveBufferSize(DEFAULT_UDP_CONNECTION_BUFFER_SIZE);
}

UdpConnection::~UdpConnection()
{
}

std::string UdpConnection::GetPeerAddress() const
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return "";
    }

    return mSocket->PeerAddress.cref();
}

uint16_t UdpConnection::GetPeerPort() const
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return 0;
    }

    return mSocket->PeerPort.cref();
}

void UdpConnection::SetPeerAddress(const std::string& peerAddress)
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return;
    }

    mSocket->PeerAddress.set(peerAddress);
}

void UdpConnection::SetPeerPort(uint16_t peerPort)
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return;
    }

    mSocket->PeerPort.set(peerPort);
}

bool UdpConnection::DataReady()
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        Close();
        return false;
    }

    bool result = mSocket->ReceiveWait(ReceiveTimeout.cref());
    if (result)
    {
        mDataRetrieved = false;
    }

    return result;
}

uint64_t UdpConnection::Receive(char* buffer, int32_t length)
{
    if (buffer == nullptr || length <= 0)
    {
        return 0;
    }

    int32_t received = 0;
    std::string source;
    uint16_t port = 0;

    if (mSocket->ReceiveFrom(&buffer[0], length, source, port, received))
    {
        mDataRetrieved = true;
    }

    return received;
}

uint64_t UdpConnection::Send(const char* buffer, int32_t length)
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return 0;
    }

    if (buffer == nullptr || length <= 0)
    {
        return 0;
    }

    try
    {
        if (mSocket->SendWait(SendTimeout.cref()))
        {
            int32_t sent = 0;
            if (mSocket->SendTo(&buffer[0], length, mSocket->PeerAddress.cref(), mSocket->PeerPort.cref(), sent))
            {
                return sent;
            }
        }
    }
    catch (SocketException& e)
    {
        // Do not need to close the UDP connection.
        LOG("Failed to send: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
    }

    return 0;
}

bool UdpConnection::IsClosed()
{
    // UDP is not connection oriented, the connection is always open.
    return mDataRetrieved;
}

bool UdpConnection::Close()
{
    // UDP is not connection oriented.
    return true;
}
