#include "Client.h"
#include "Connection.h"
#include "NetworkUtility.h"
#include "Reactor.h"

using namespace microreactor;


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

void Client::Initialize(std::shared_ptr<Connection> connection)
{
    mConnection = connection;

    if (mConnection->IsClosed())
    {
        ClientName->clear();
    }
    else
    {
        // Push to the queue to receive messages.
        if (mConnection->Start())
        {
            ClientName(mConnection->Name());
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
        mConnection = NetworkUtility::CreateConnection(mConnection->Dispatcher());
    }

    return mConnection;
}
