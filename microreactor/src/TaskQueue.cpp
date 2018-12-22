#include "TaskQueue.h"

using namespace sg::microreactor;


TaskQueue::TaskQueue()
{
}

TaskQueue::~TaskQueue()
{
    ClearQueue();
}

void TaskQueue::Start()
{
    mStop = false;
}

void TaskQueue::Stop()
{
    mStop = true;
}

void TaskQueue::ClearQueue()
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    mTaskQueue.clear();
}

uint64_t TaskQueue::GetQueueDepth()
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    return GetTaskQueueDepth();
}

uint64_t TaskQueue::GetTaskQueueDepth()
{
    return mTaskQueue.size();
}

bool TaskQueue::EnqueueTask(TaskPtr task)
{
    if (mStop || task == nullptr)
    {
        // Stop inserting into the task queue
        return false;
    }

    mTaskQueue.emplace_back(task);

    task->Schedule();
    return true;
}

TaskPtr TaskQueue::DequeueTask()
{
    if (mStop || mTaskQueue.empty() || GetTaskQueueDepth() == 0)
    {
        return nullptr;
    }
    
    TaskPtr task = nullptr;
    do
    {
        // Remove the task from the queue
        task = std::move(mTaskQueue.front());
        mTaskQueue.pop_front();
    } while (task == nullptr);
    
    // Move the task to the active pool
    mActivePool.emplace(task);

    return task;
}

bool TaskQueue::Submit(TaskPtr task)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    bool result = EnqueueTask(task);
    if (result)
    {
        mTaskSubmitted.notify_one();
    }
    return result;
}

TaskPtr TaskQueue::GetTask(const std::chrono::milliseconds& timeout)
{
    if (timeout.count() == 0)
    {
        std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
        return DequeueTask();
    }

    // The condition variable requires a unique_lock for waiting, otherwise use lock_guard.
    std::unique_lock<decltype(mTaskQueueMutex)> uniqueLock(mTaskQueueMutex);
    bool result = true;
    
    while (result)
    {
        TaskPtr task = DequeueTask();
        if (task != nullptr)
        {
            return task;
        }

        result = std::cv_status::no_timeout == mTaskSubmitted.wait_for(uniqueLock, timeout);            
    }
    
    return nullptr;
}

uint64_t TaskQueue::GetActiveTaskCount()
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);

    // Get active task in the active pool
    return mActivePool.size();
}

uint64_t TaskQueue::CancelTasks()
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    uint64_t cancelCount = 0;

    // Cancel tasks in the active pool
    for (auto activeTask : mActivePool)
    {
        activeTask->Cancel();
        cancelCount++;
    }

    // Cancel and remove tasks in the queue
    for (auto task : mTaskQueue)
    {
        task->Cancel();
        cancelCount++;
    }

    mTaskQueue.clear();
    return cancelCount;
}

void TaskQueue::CompleteTask(TaskPtr task)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);

    // Remove tasks in the active pool
    mActivePool.erase(task);
}
