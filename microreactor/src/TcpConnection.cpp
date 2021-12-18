#include "TcpConnection.h"
#include <algorithm>
#include "Exception.h"
#include "TcpSocket.h"

using namespace microreactor;


TcpConnection::TcpConnection(std::shared_ptr<TcpSocket> socket, std::shared_ptr<microreactor::Dispatcher> dispatcher)
    : mSocket(socket)
{
    Dispatcher(dispatcher);

    if (mSocket == nullptr || !mSocket->IsValid())
    {
        // Only for client connection
        if (mSocket == nullptr)
        {
            mSocket = std::make_shared<TcpSocket>();
        }

        EnsureClientConnection();
    }

    if (!mSocket->HostAddress->empty())
    {
        Name(std::string("[") + mSocket->HostAddress() + "]:" + std::to_string(mSocket->HostPort()));
    }

    mReceiveBufferSize = DEFAULT_TCP_CONNECTION_BUFFER_SIZE;
    mSocket->SetReceiveBufferSize(DEFAULT_TCP_CONNECTION_BUFFER_SIZE);
    mSocket->SetNonblocking(true);
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

    return mSocket->PeerAddress();
}

uint16_t TcpConnection::GetPeerPort() const
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        return 0;
    }

    return mSocket->PeerPort();
}

uint64_t TcpConnection::Receive(char* buffer, int length)
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
        int received = 0;
        if (mSocket->ReceiveWait(std::chrono::milliseconds::zero()))
        {
            if (mSocket->Receive(&buffer[0], length, received))
            {
                return received;
            }
        }
    }
    catch (SocketException& e)
    {
        LOG("Failed to receive: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
        Close();
    }

    return 0;
}

uint64_t TcpConnection::Send(const char* buffer, int length)
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
        if (mSocket->SendWait(SendTimeout()))
        {
            int sent = 0;
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
            LOG("Close connection: [%s]:%u", mSocket->PeerAddress->c_str(), mSocket->PeerPort());
            mSocket->Detach();
        }
        
        mSocket = nullptr;
    }

    return true;
}

bool TcpConnection::EnsureClientConnection()
{
    if (mSocket == nullptr)
    {
        Close();
        return false;
    }

    if (!mSocket->IsValid())
    {
        try
        {
            mSocket->Connect(Dispatcher()->Address(), Dispatcher()->Port());
            if (!mSocket->IsValid())
            {
                LOG("Failed to connect to [%s]:%u", Dispatcher()->Address->c_str(), Dispatcher()->Port());
                return false;
            }
        }
        catch (SocketException& e)
        {
            LOG("Failed to connect: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
            return false;
        }

        Name(std::string("[") + mSocket->HostAddress() + "]:" + std::to_string(mSocket->HostPort()));

        mReceiveBufferSize = DEFAULT_TCP_CONNECTION_BUFFER_SIZE;
        mSocket->SetReceiveBufferSize(mReceiveBufferSize);
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
