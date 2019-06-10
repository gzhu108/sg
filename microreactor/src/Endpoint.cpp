#include "Endpoint.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;


Endpoint::Endpoint()
{
}

Endpoint::~Endpoint()
{
    mAcceptTask->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
    mAcceptTask->Cancel();

    CloseAllConnections();
}

bool Endpoint::Start()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    // Start listening on the endpoint
    mAcceptTask = SUBMIT(std::bind(&Endpoint::AcceptConnection, this), "Endpoint::AcceptConnection");
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

bool Endpoint::Stop()
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

void Endpoint::AcceptConnection()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (IsClosed())
    {
        return;
    }

    auto listenTimeout = ListenTimeout.cref();
    std::shared_ptr<Connection> connection = Listen(listenTimeout);
    if (connection != nullptr)
    {
        connection->ReceiveTimeout.set(ReceiveTimeout.cref());
        connection->SendTimeout.set(SendTimeout.cref());

        AddConnection(connection);

        // Signal a connection is made
        mConnectionMade(connection);

        // Push to the queue to receive messages.
        connection->Start();
    }
}

void Endpoint::AddConnection(std::shared_ptr<Connection> connection)
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

void Endpoint::RemoveConnection(std::shared_ptr<Connection> connection)
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

void Endpoint::CloseAllConnections()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    for (auto& connection : mActiveConnections)
    {
        connection->Closed.Disconnect(reinterpret_cast<uintptr_t>(this));
        connection->Stop();
    }

    mActiveConnections.clear();
}

bool Endpoint::GetAllConnections(std::set<std::shared_ptr<Connection>>& connections)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);
    connections = mActiveConnections;
    return !connections.empty();
}
