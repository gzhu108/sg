#include "UdpEndpoint.h"
#include "UdpSocket.h"
#include "UdpConnection.h"
#include "Exception.h"

using namespace sg::microreactor;


UdpEndpoint::UdpEndpoint(std::shared_ptr<UdpSocket> socket, std::shared_ptr<Profile> profile)
    : Endpoint(profile)
    , mSocket(socket)
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        mSocket = std::make_shared<UdpSocket>();
    }

    if (mProfile != nullptr)
    {
        try
        {
            mSocket->Bind(mProfile->Address->c_str(), mProfile->Port.cref());
            mConnection = std::make_shared<UdpConnection>(mSocket, mProfile);
            Name.set(std::string("[") + mSocket->HostName.cref() + "]:" + std::to_string(mSocket->HostPort.cref()));
        }
        catch (SocketException& e)
        {
            // Socket exception received.
            LOG("Failed to bind: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
            Close();
        }
    }
    else
    {
        Name.set(std::string("[") + mSocket->HostName.cref() + "]:" + std::to_string(mSocket->HostPort.cref()));
    }
}

UdpEndpoint::~UdpEndpoint()
{
    Close();
}

bool UdpEndpoint::IsClosed()
{
    return mSocket == nullptr;
}

bool UdpEndpoint::Close()
{
    if (mSocket != nullptr)
    {
        LOG("Close host: %s:%u", mSocket->HostName->c_str(), mSocket->HostPort.cref());
        mSocket->Detach();
        mSocket = nullptr;
    }
    return true;
}

std::shared_ptr<Connection> UdpEndpoint::Listen(const std::chrono::milliseconds& timeout)
{
    if (mSocket == nullptr || !mSocket->IsValid() || mConnection == nullptr)
    {
        Close();
        return nullptr;
    }

    try
    {
        if (mConnection->DataReady())
        {
            return mConnection;
        }
    }
    catch (SocketException& e)
    {
        // Socket exception received.
        LOG("Failed to listen: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
    }
    
    return nullptr;
}
