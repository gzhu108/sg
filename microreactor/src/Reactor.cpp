#include "Reactor.h"

using namespace sg::microreactor;


Reactor::Reactor(Connection& connection)
    : mConnection(connection)
{
}

Reactor::~Reactor()
{
    RemoveAllTasks();
}

void Reactor::Stop()
{
    RemoveAllTasks();
}

void Reactor::CheckComplete()
{
    if (mActiveTasks.empty())
    {
        mCompleted();
    }
}

void Reactor::AddTask(TaskPtr task)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (task != nullptr)
    {
        mActiveTasks.emplace(task);

        task->Completed.Connect([=]()
        {
            RemoveTask(task);
        }, reinterpret_cast<uintptr_t>(this));

        task->Canceled.Connect([=]()
        {
            RemoveTask(task);
        }, reinterpret_cast<uintptr_t>(this));
    }
}

void Reactor::RemoveTask(TaskPtr task)
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    if (task != nullptr)
    {
        auto found = mActiveTasks.find(task);
        if (found != mActiveTasks.end())
        {
            task->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
            task->Canceled.Disconnect(reinterpret_cast<uintptr_t>(this));

            // The order of the calls is important
            mActiveTasks.erase(task);
            task->Cancel();
        }
    }

    CheckComplete();
}

void Reactor::RemoveAllTasks()
{
    ScopeLock<decltype(mLock)> scopeLock(mLock);

    for (auto& task : mActiveTasks)
    {
        task->Completed.Disconnect(reinterpret_cast<uintptr_t>(this));
        task->Canceled.Disconnect(reinterpret_cast<uintptr_t>(this));
        task->Cancel();
    }

    mActiveTasks.clear();
}
