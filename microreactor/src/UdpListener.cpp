#include "UdpListener.h"
#include "UdpSocket.h"
#include "UdpConnection.h"
#include "Exception.h"

using namespace microreactor;


UdpListener::UdpListener(std::shared_ptr<UdpSocket> socket, std::shared_ptr<microreactor::Dispatcher> dispatcher)
    : mSocket(socket)
{
    Dispatcher(dispatcher);

    if (mSocket == nullptr)
    {
        mSocket = std::make_shared<UdpSocket>();
    }

    try
    {
        if (dispatcher != nullptr)
        {
            mSocket->Bind(dispatcher->Address->c_str(), dispatcher->Port());
        }
    }
    catch (SocketException& e)
    {
        // Socket exception received.
        LOG("Failed to bind: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
        Close();
    }

    if (dispatcher != nullptr)
    {
        mConnection = std::make_shared<UdpConnection>(mSocket, dispatcher);
        Name(std::string("[") + mSocket->HostAddress() + "]:" + std::to_string(mSocket->HostPort()));
    }
    else
    {
        Name(std::string("[") + mSocket->HostAddress() + "]:" + std::to_string(mSocket->HostPort()));
    }
}

UdpListener::~UdpListener()
{
    Close();
}

bool UdpListener::IsClosed()
{
    return mSocket == nullptr;
}

bool UdpListener::Close()
{
    if (mSocket != nullptr)
    {
        LOG("Close host: %s:%u", mSocket->HostAddress->c_str(), mSocket->HostPort());
        mSocket->Detach();
        mSocket = nullptr;
    }
    return true;
}

std::shared_ptr<Connection> UdpListener::Listen()
{
    if (mSocket == nullptr || !mSocket->IsValid() || mConnection == nullptr)
    {
        Close();
        return nullptr;
    }

    return mConnection;
}
