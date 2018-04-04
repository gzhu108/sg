#include "Endpoint.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;


std::map<std::shared_ptr<Connection>, Endpoint*> Endpoint::mActiveConnections;
std::recursive_mutex Endpoint::mActiveConnectionsLock;


Endpoint::Endpoint(std::shared_ptr<Profile> profile)
    : mProfile(profile)
{
}

Endpoint::~Endpoint()
{
    ScopeLock<decltype(mActiveConnectionsLock)> scopeLock(mActiveConnectionsLock);
    for (auto& connection : mActiveConnections)
    {
        if (connection.second == this)
        {
            connection.first->Closed.Disconnect(reinterpret_cast<uintptr_t>(this));
            connection.first->Stop();
        }
    }

    CancelAllTasks(ListenTimeout.cref());
    
    // Do not clear connections as the mActiveConnections is shared with all endpoint.
    //mActiveConnections.clear();
}

bool Endpoint::Start()
{
    // Start listening on the endpoint
    SUBMIT(std::bind(&Endpoint::AcceptConnection, this), shared_from_this(), this, "Endpoint::AcceptConnection");
    return true;
}

bool Endpoint::Stop()
{
    if (!IsClosed())
    {
        Close();
    }

    CancelAllTasks(ListenTimeout.cref());

    return true;
}

void Endpoint::AcceptConnection()
{
    if (IsClosed())
    {
        return;
    }

    // Remove all closed connections
    RemoveClosedConnections();

    auto listenTimeout = ListenTimeout.cref();
    {
        ScopeLock<decltype(mActiveConnectionsLock)> scopeLock(mActiveConnectionsLock);
        if (mActiveConnections.empty())
        {
            listenTimeout = std::chrono::milliseconds(100);
        }
    }

    std::shared_ptr<Connection> connection = Listen(listenTimeout);
    if (connection != nullptr)
    {
        {
            ScopeLock<decltype(mActiveConnectionsLock)> scopeLock(mActiveConnectionsLock);
            mActiveConnections.emplace(connection, this);
        }

        connection->ReceiveTimeout.set(ReceiveTimeout.cref());
        connection->SendTimeout.set(SendTimeout.cref());

        connection->Closed.Connect([&]
        {
            ScopeLock<decltype(mActiveConnectionsLock)> scopeLock(mActiveConnectionsLock);
            mActiveConnections.erase(connection);
        }, reinterpret_cast<uintptr_t>(this));

        // Signal a connection is made
        mConnectionMade(connection);

        // Push to the queue to receive messages.
        connection->Start();
    }

    if (!IsClosed())
    {
        SUBMIT(std::bind(&Endpoint::AcceptConnection, this), shared_from_this(), this, "Endpoint::AcceptConnection");
    }
}

void Endpoint::CancelAllTasks(const std::chrono::microseconds& waitTime)
{
    uint64_t cancelCount = CANCEL_TASKS(this);

    if (waitTime.count() > 0 && cancelCount > 0)
    {
        while (GET_ACTIVE_TASK_COUNT(this) > 0)
        {
            std::this_thread::sleep_for(waitTime);
        }
    }
}

void Endpoint::RemoveClosedConnections()
{
    ScopeLock<decltype(mActiveConnectionsLock)> scopeLock(mActiveConnectionsLock);

    auto activeConnection = mActiveConnections.begin();
    while (activeConnection != mActiveConnections.end())
    {
        if (activeConnection->first->IsClosed())
        {
            activeConnection = mActiveConnections.erase(activeConnection);
        }
        else
        {
            activeConnection++;
        }
    }
}
