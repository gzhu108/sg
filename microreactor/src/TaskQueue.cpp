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
    return mTaskQueue.size();
}

bool TaskQueue::EnqueueTask(TaskPtr task, bool insertBack)
{
    if (mStop || task == nullptr || task->GetStatus() != TaskStatus::Created)
    {
        // Stop inserting into the task queue
        return false;
    }

    if (insertBack)
    {
        // Insert the task to the back of the queue
        mTaskQueue.emplace_back(task);
    }
    else
    {
        // Insert the task to the front of the queue
        mTaskQueue.emplace_front(task);
    }

    task->Schedule();
    return true;
}

TaskPtr TaskQueue::DequeueTask()
{
    if (mTaskQueue.empty())
    {
        return nullptr;
    }
    
    // Remove a task from the front
    TaskPtr task = std::move(mTaskQueue.front());
    mTaskQueue.pop_front();

    // Move the task to the active pool
    mActivePool.insert(std::make_pair(task->Owner.cref(), task));

    return task;
}

bool TaskQueue::Submit(TaskPtr task, bool insertBack)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    bool result = EnqueueTask(task, insertBack);
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

uint64_t TaskQueue::GetActiveTaskCount(uintptr_t owner)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);

    // Get active task in the active pool
    auto range = std::make_pair(mActivePool.begin(), mActivePool.end());
    if (owner != 0)
    {
        range = mActivePool.equal_range(owner);
    }

    return std::distance(range.first, range.second);
}

uint64_t TaskQueue::CancelTasks(uintptr_t owner)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    uint64_t cancelCount = 0;

    // Determine how many active tasks to cancel
    auto range = std::make_pair(mActivePool.begin(), mActivePool.end());
    if (owner != 0)
    {
        range = mActivePool.equal_range(owner);
    }

    // Cancel tasks in the active pool
    for (auto task = range.first; task != range.second; task++)
    {
        task->second->Cancel();
        cancelCount++;
    }

    // Cancel and remove tasks in the queue
    auto task = mTaskQueue.begin();
    while (task != mTaskQueue.end())
    {
        if (owner == 0 || owner == (*task)->Owner.cref())
        {
            (*task)->Cancel();
            task = mTaskQueue.erase(task);
            cancelCount++;
        }
        else
        {
            task++;
        }
    }

    return cancelCount;
}

void TaskQueue::CompleteTask(TaskPtr task)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);

    // Remove tasks in the active pool
    auto range = mActivePool.equal_range(task->Owner.cref());
    for (auto activeTask = range.first; activeTask != range.second; activeTask++)
    {
        if (activeTask->second == task)
        {
            mActivePool.erase(activeTask);
            break;
        }
    }
}
