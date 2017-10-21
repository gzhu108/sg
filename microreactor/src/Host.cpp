#include "Host.h"
#include "Dispatcher.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;


std::map<std::shared_ptr<Connection>, Host*> Host::mActiveConnections;
std::recursive_mutex Host::mActiveConnectionsLock;


Host::Host(std::shared_ptr<Profile> profile)
    : mProfile(profile)
{
}

Host::~Host()
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
    
    // Do not clear connections as the mActiveConnections is shared with all host.
    //mActiveConnections.clear();
}

bool Host::Start()
{
    // Start listen on the host
    SUBMIT(std::bind(&Host::AcceptConnection, this), reinterpret_cast<uintptr_t>(this), "Host::AcceptConnection");
    return true;
}

bool Host::Stop()
{
    if (!IsClosed())
    {
        Close();
    }

    CancelAllTasks(ListenTimeout.cref());

    return true;
}

void Host::AcceptConnection()
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

        mConnectionMade(connection);

        // Push to the queue to receive messages.
        connection->Start();
    }

    if (!IsClosed())
    {
        SUBMIT(std::bind(&Host::AcceptConnection, this), reinterpret_cast<uintptr_t>(this), "Host::AcceptConnection");
    }
}

void Host::CancelAllTasks(const std::chrono::microseconds& waitTime)
{
    uint64_t cancelCount = CANCEL_TASKS(reinterpret_cast<uintptr_t>(this));

    if (waitTime.count() > 0 && cancelCount > 0)
    {
        while (GET_ACTIVE_TASK_COUNT(reinterpret_cast<uintptr_t>(this)) > 0)
        {
            std::this_thread::sleep_for(waitTime);
        }
    }
}

void Host::RemoveClosedConnections()
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
