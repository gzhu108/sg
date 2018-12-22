#include "TaskCategorizedQueue.h"

using namespace sg::microreactor;


TaskCategorizedQueue::TaskCategorizedQueue()
{
}

TaskCategorizedQueue::~TaskCategorizedQueue()
{
}

void TaskCategorizedQueue::ClearQueue()
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    mCategoryQueue.clear();
    mTaskCategorizedQueue.clear();
}

uint64_t TaskCategorizedQueue::GetTaskQueueDepth()
{
    uint64_t queueDepth = 0;
    for (const auto& TaskCategorizedQueue : mTaskCategorizedQueue)
    {
        queueDepth += TaskCategorizedQueue->size();
    }

    return queueDepth;
}

bool TaskCategorizedQueue::EnqueueTask(TaskPtr task, int64_t category)
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
        mTaskCategorizedQueue.emplace_back(&mCategoryQueue[category]);
    }
    else
    {
        categoryQueue->second.emplace_back(task);
    }

    task->Schedule();
    return true;
}

TaskPtr TaskCategorizedQueue::DequeueTask()
{
    if (mStop || mTaskCategorizedQueue.empty() || GetTaskQueueDepth() == 0)
    {
        return nullptr;
    }
    
    TaskPtr task = nullptr;
    do
    {
        auto TaskCategorizedQueue = std::move(mTaskCategorizedQueue.front());
        if (!TaskCategorizedQueue->empty())
        {
            // Remove the task from the queue
            task = std::move(TaskCategorizedQueue->front());
            TaskCategorizedQueue->pop_front();
        }

        // Move the queue to the back
        mTaskCategorizedQueue.pop_front();
        mTaskCategorizedQueue.emplace_back(TaskCategorizedQueue);
    } while (task == nullptr);
    
    // Move the task to the active pool
    mActivePool.emplace(task);

    return task;
}

bool TaskCategorizedQueue::Submit(TaskPtr task, int64_t category)
{
    std::lock_guard<decltype(mTaskQueueMutex)> scopeLock(mTaskQueueMutex);
    bool result = EnqueueTask(task, category);
    if (result)
    {
        mTaskSubmitted.notify_one();
    }
    return result;
}

uint64_t TaskCategorizedQueue::CancelTasks()
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
    for (auto taskQueue : mTaskCategorizedQueue)
    {
        auto task = taskQueue->begin();
        while (task != taskQueue->end())
        {
            (*task)->Cancel();
            task = taskQueue->erase(task);
            cancelCount++;
        }
    }

    return cancelCount;
}
