#include "Client.h"
#include "Connection.h"
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
        connection->ReceiveTimeout.set(timeout);
        connection->Start();

        ClientName.set(connection->Name.cref());
        if (!ClientName->empty())
        {
            LOG("Client receive message from [%s]:%u", connection->GetPeerName().c_str(), connection->GetPeerPort());
        }
    }
}
