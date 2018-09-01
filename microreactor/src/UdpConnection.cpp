#include "UdpConnection.h"
#include <algorithm>
#include "Exception.h"
#include "UdpSocket.h"

using namespace sg::microreactor;


UdpConnection::UdpConnection(std::shared_ptr<UdpSocket> socket, std::shared_ptr<Profile> profile)
    : Connection(profile)
    , mSocket(socket)
{
    if (mSocket == nullptr)
    {
        mSocket = std::make_shared<UdpSocket>();
    }

    try
    {
        mSocket->Bind(mProfile->Address->c_str(), mProfile->Port.cref());
    }
    catch (SocketException& e)
    {
        // Socket exception received.
        LOG("Failed to bind: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
        Close();
    }

    if (!mSocket->HostName->empty())
    {
        Name.set(std::string("[") + mSocket->HostName.cref() + "]:" + std::to_string(mSocket->HostPort.cref()));
    }

    mSocket->SetReceiveBufferSize(DEFAULT_UDP_CONNECTION_BUFFER_SIZE);
    mReceiveBuffer.resize(DEFAULT_UDP_CONNECTION_BUFFER_SIZE);
}

UdpConnection::~UdpConnection()
{
}

std::string UdpConnection::GetPeerName() const
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return "";
    }

    return mSocket->PeerName.cref();
}

uint16_t UdpConnection::GetPeerPort() const
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return 0;
    }

    return mSocket->PeerPort.cref();
}

void UdpConnection::SetPeerName(const std::string& peerName)
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return;
    }

    mSocket->PeerName.set(peerName);
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
            if (mSocket->SendTo(&buffer[0], length, mSocket->PeerName.cref(), mSocket->PeerPort.cref(), sent))
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
