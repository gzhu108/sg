#include "Endpoint.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;


Endpoint::Endpoint(std::shared_ptr<Profile> profile)
    : mProfile(profile)
{
}

Endpoint::~Endpoint()
{
    CancelAllTasks(ListenTimeout.cref());
    
    // Do not clear connections as the mActiveConnections is shared with all endpoint.
    //mActiveConnections.clear();
}

bool Endpoint::Start()
{
    // Start listening on the endpoint
    auto task = SUBMIT_MEMBER(Endpoint::AcceptConnection, "Endpoint::AcceptConnection");
    if (task != nullptr)
    {
        task->Completed.Connect([&, task]()
        {
            task->Schedule();
        });

        return true;
    }

    return false;
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

    auto listenTimeout = ListenTimeout.cref();
    std::shared_ptr<Connection> connection = Listen(listenTimeout);
    if (connection != nullptr)
    {
        connection->ReceiveTimeout.set(ReceiveTimeout.cref());
        connection->SendTimeout.set(SendTimeout.cref());

        // Signal a connection is made
        mConnectionMade(connection);

        // Push to the queue to receive messages.
        connection->Start();
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
