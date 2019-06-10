#include "TcpEndpoint.h"
#include "TcpSocket.h"
#include "TcpConnection.h"
#include "Exception.h"

using namespace sg::microreactor;


TcpEndpoint::TcpEndpoint(std::shared_ptr<TcpSocket> socket, std::shared_ptr<sg::microreactor::Dispatcher> dispatcher)
    : mSocket(socket)
{
    Dispatcher.set(dispatcher);

    if (mSocket == nullptr)
    {
        mSocket = std::make_shared<TcpSocket>();
    }

    if (dispatcher == nullptr)
    {
        LOG("Failed to listen: invalid profile");
        Close();
        return;
    }
    else
    {
        try
        {
            mSocket->Listen(dispatcher->Address->c_str(), dispatcher->Port.cref());
        }
        catch (SocketException& e)
        {
            // Socket exception received.
            LOG("Failed to listen: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
            Close();
            return;
        }
    }

    Name.set(std::string("[") + mSocket->HostAddress.cref() + "]:" + std::to_string(mSocket->HostPort.cref()));
}

TcpEndpoint::~TcpEndpoint()
{
    Close();
}

bool TcpEndpoint::IsClosed()
{
    return mSocket == nullptr;
}

bool TcpEndpoint::Close()
{
    if (mSocket != nullptr)
    {
        LOG("Close endpoint: %s:%u", mSocket->HostAddress->c_str(), mSocket->HostPort.cref());
        mSocket->Detach();
        mSocket = nullptr;
    }
    return true;
}

std::shared_ptr<Connection> TcpEndpoint::Listen(const std::chrono::milliseconds& timeout)
{
    if (mSocket == nullptr || !mSocket->IsValid())
    {
        Close();
        return nullptr;
    }

    try
    {
        std::shared_ptr<TcpSocket> clientSocket = mSocket->Accept(timeout);
        if (clientSocket != nullptr)
        {
            std::shared_ptr<TcpConnection> connection = std::make_shared<TcpConnection>(clientSocket, Dispatcher.cref());
            return connection;
        }
    }
    catch (SocketException& e)
    {
        // Socket exception received.
        LOG("Failed to accept connection: exception(%d): %s [%s]:%u", e.mError, e.what(), e.mName.c_str(), e.mPort);
    }
    
    return nullptr;
}
