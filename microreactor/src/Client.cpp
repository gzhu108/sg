#include "Client.h"
#include "Connection.h"
#include "NetworkUtility.h"
#include "Reactor.h"

using namespace sg::microreactor;


Client::Client()
{
}

Client::~Client()
{
    if (mConnection != nullptr)
    {
        mConnection->Stop();
        mConnection = nullptr;
    }
}

void Client::Initialize(std::shared_ptr<Connection> connection, const std::chrono::milliseconds& timeout)
{
    mConnection = connection;

    if (mConnection == nullptr || mConnection->IsClosed())
    {
        ClientName->clear();
    }
    else
    {
        // Push to the queue to receive messages.
        mConnection->ReceiveTimeout.set(timeout);
        if (mConnection->Start())
        {
            ClientName.set(mConnection->Name.cref());
            if (!ClientName->empty())
            {
                LOG("Client receive message from %s --> [%s:%d]", ClientName->c_str(), mConnection->GetPeerAddress().c_str(), mConnection->GetPeerPort());
            }
        }
        else
        {
            LOG("Client failed to start [%s] --> [%s:%d]", ClientName->c_str(), mConnection->GetPeerAddress().c_str(), mConnection->GetPeerPort());
        }
    }
}

std::shared_ptr<Connection> Client::GetConnection()
{
    if (mConnection == nullptr)
    {
        return nullptr;
    }
    else if (mConnection->IsClosed())
    {
        mConnection = NetworkUtility::CreateConnection(mConnection->GetProfile());
    }

    return mConnection;
}
