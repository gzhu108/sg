#include "TcpConnection.h"
#include <algorithm>
#include "Exception.h"
#include "TcpSocket.h"

using namespace sg::microreactor;


TcpConnection::TcpConnection(std::shared_ptr<TcpSocket> socket, std::shared_ptr<Profile> profile)
    : Connection(profile)
    , mSocket(socket)
    , mServerConnection(false)
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
            mSocket = std::make_shared<TcpSocket>();
        }

        // Client connection to a server
        mServerConnection = false;
    }

    if (!mSocket->HostAddress->empty())
    {
        Name.set(std::string("[") + mSocket->HostAddress.cref() + "]:" + std::to_string(mSocket->HostPort.cref()));
    }

    mReceiveBufferSize = DEFAULT_TCP_CONNECTION_BUFFER_SIZE;
    mSocket->SetReceiveBufferSize(DEFAULT_TCP_CONNECTION_BUFFER_SIZE);
}

TcpConnection::~TcpConnection()
{
    Close();
}

std::string TcpConnection::GetPeerAddress() const
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return "";
    }

    return mSocket->PeerAddress.cref();
}

uint16_t TcpConnection::GetPeerPort() const
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return 0;
    }

    return mSocket->PeerPort.cref();
}

bool TcpConnection::DataReady()
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        Close();
        return false;
    }

    return mSocket->ReceiveWait(ReceiveTimeout.cref());
}

uint64_t TcpConnection::Receive(char* buffer, int32_t length)
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        Close();
        return false;
    }

    if (buffer == nullptr || length <= 0)
    {
        return 0;
    }

    try
    {
        int32_t received = 0;
        if (mSocket->Receive(&buffer[0], length, received))
        {
            return received;
        }
    }
    catch (SocketException& e)
    {
        LOG("Failed to receive: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
        Close();
    }

    return 0;
}

uint64_t TcpConnection::Send(const char* buffer, int32_t length)
{
    if (!EnsureConnection())
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
            if (mSocket->Send(&buffer[0], length, sent))
            {
                return sent;
            }
        }
    }
    catch (SocketException& e)
    {
        LOG("Failed to send: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
        Close();
    }

    return 0;
}

bool TcpConnection::IsClosed()
{
    return mSocket == nullptr;
}

bool TcpConnection::Close()
{
    // It's possible for both send and receive to call Close at same time.
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (mSocket != nullptr)
    {
        if (mSocket->IsValid())
        {
            LOG("Close connection: [%s]:%u", mSocket->PeerAddress->c_str(), mSocket->PeerPort.cref());
            mSocket->Detach();
        }
        
        if (mServerConnection)
        {
            mSocket = nullptr;
        }
        else
        {
            mSocket = std::make_shared<TcpSocket>();
        }
    }

    return true;
}

bool TcpConnection::EnsureConnection()
{
    if (mSocket == nullptr)
    {
        Close();
        return false;
    }

    if (!mSocket->IsValid())
    {
        // Only try to reconnect in client connection.
        if (mServerConnection)
        {
            return false;
        }

        try
        {
            mSocket->Connect(mProfile->Address.cref(), mProfile->Port.cref(), SendTimeout.cref());
            if (!mSocket->IsValid())
            {
                LOG("Failed to connect to [%s]:%u", mProfile->Address->c_str(), mProfile->Port.cref());
                return false;
            }
        }
        catch (SocketException& e)
        {
            LOG("Failed to connect: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
            return false;
        }

        Name.set(std::string("[") + mSocket->HostAddress.cref() + "]:" + std::to_string(mSocket->HostPort.cref()));

        mReceiveBufferSize = DEFAULT_TCP_CONNECTION_BUFFER_SIZE;
        mSocket->SetReceiveBufferSize(mReceiveBufferSize);

        // Start receiving data from the connected socket
        Start();
    }

    return true;
}

void TcpConnection::SetReceiveBufferSize(PROPERTY_TYPE(ReceiveBufferSize)& propertyValue, const uint32_t& value)
{
    if (mSocket != nullptr && propertyValue != value)
    {
        mReceiveBufferSize = value;
        mSocket->SetReceiveBufferSize(value);
        propertyValue = value;
    }
}
