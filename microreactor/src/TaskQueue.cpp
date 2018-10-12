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
    mCategoryQueue.clear();
    mTaskQueue.clear();
}

uint64_t TaskQueue::GetQueueDepth()
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    return GetTaskQueueDepth();
}

uint64_t TaskQueue::GetTaskQueueDepth()
{
    uint64_t queueDepth = 0;
    for (const auto& taskQueue : mTaskQueue)
    {
        queueDepth += taskQueue->size();
    }

    return queueDepth;
}

bool TaskQueue::EnqueueTask(TaskPtr task, int64_t category)
{
    if (mStop || task == nullptr)
    {
        // Stop inserting into the task queue
        return false;
    }

    auto categoryQueue = mCategoryQueue.find(category);
    if (categoryQueue == mCategoryQueue.end())
    {
        mCategoryQueue[category].emplace_back(task);
        mTaskQueue.emplace_back(&mCategoryQueue[category]);
    }
    else
    {
        categoryQueue->second.emplace_back(task);
    }

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
        auto taskQueue = std::move(mTaskQueue.front());
        if (!taskQueue->empty())
        {
            // Remove the task from the queue
            task = std::move(taskQueue->front());
            taskQueue->pop_front();
        }

        // Move the queue to the back
        mTaskQueue.pop_front();
        mTaskQueue.emplace_back(taskQueue);
    } while (task == nullptr);
    
    // Move the task to the active pool
    mActivePool.insert(std::make_pair(task->ActiveId.cref(), task));

    return task;
}

bool TaskQueue::Submit(TaskPtr task, int64_t category)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    bool result = EnqueueTask(task, category);
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

uint64_t TaskQueue::CancelTasks(uintptr_t activeId)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    uint64_t cancelCount = 0;

    // Determine how many active tasks to cancel
    auto range = std::make_pair(mActivePool.begin(), mActivePool.end());
    if (activeId != 0)
    {
        range = mActivePool.equal_range(activeId);
    }

    // Cancel tasks in the active pool
    for (auto task = range.first; task != range.second; task++)
    {
        task->second->Cancel();
        cancelCount++;
    }

    // Cancel and remove tasks in the queue
    for (auto taskQueue : mTaskQueue)
    {
        auto task = taskQueue->begin();
        while (task != taskQueue->end())
        {
            if (activeId == 0 || activeId == (*task)->ActiveId.cref())
            {
                (*task)->Cancel();
                task = taskQueue->erase(task);
                cancelCount++;
            }
            else
            {
                task++;
            }
        }
    }

    return cancelCount;
}

void TaskQueue::CompleteTask(TaskPtr task)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);

    // Remove tasks in the active pool
    auto range = mActivePool.equal_range(task->ActiveId.cref());
    for (auto activeTask = range.first; activeTask != range.second; activeTask++)
    {
        if (activeTask->second == task)
        {
            mActivePool.erase(activeTask);
            break;
        }
    }
}
