#include "Listener.h"
#include "TaskManagerSingleton.h"

using namespace microreactor;


Listener::Listener()
{
}

Listener::~Listener()
{
    mAcceptTask->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
    mAcceptTask->Cancel();

    CloseAllConnections();
}

bool Listener::Start()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Start listening on the listener
    mAcceptTask = SUBMIT(std::bind(&Listener::AcceptConnection, this), "Listener::AcceptConnection");
    if (mAcceptTask != nullptr)
    {
        auto taskRawPtr = mAcceptTask.get();
        mAcceptTask->Completed.Connect([&, taskRawPtr]()
        {
            taskRawPtr->Schedule();
        }, reinterpret_cast<uintptr_t>(this));

        return true;
    }

    return false;
}

bool Listener::Stop()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (!IsClosed())
    {
        Close();
    }

    mAcceptTask->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
    mAcceptTask->Cancel();

    CloseAllConnections();

    return true;
}

void Listener::AcceptConnection()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (IsClosed())
    {
        return;
    }

    std::shared_ptr<Connection> connection = Listen();
    if (connection != nullptr)
    {
        connection->SendTimeout.set(SendTimeout.cref());
        AddConnection(connection);

        // Signal a connection is made
        mConnectionMade(connection);

        // Push to the queue to receive messages.
        connection->Start();
    }
}

void Listener::AddConnection(std::shared_ptr<Connection> connection)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (connection != nullptr)
    {
        connection->Closed.Connect([=]()
        {
            RemoveConnection(connection);
        }, reinterpret_cast<uintptr_t>(this));

        mActiveConnections.emplace(connection);
    }
}

void Listener::RemoveConnection(std::shared_ptr<Connection> connection)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (connection != nullptr)
    {
        auto found = mActiveConnections.find(connection);
        if (found != mActiveConnections.end())
        {
            connection->Closed.Disconnect(reinterpret_cast<uintptr_t>(this));

            // The order of the calls is important
            mActiveConnections.erase(found);
            connection->Stop();
        }
    }
}

void Listener::CloseAllConnections()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    for (auto& connection : mActiveConnections)
    {
        connection->Closed.Disconnect(reinterpret_cast<uintptr_t>(this));
        connection->Stop();
    }

    mActiveConnections.clear();
}

bool Listener::GetAllConnections(std::set<std::shared_ptr<Connection>>& connections)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    connections = mActiveConnections;
    return !connections.empty();
}
