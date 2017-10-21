#include "ThreadPool.h"

using namespace sg::microreactor;


ThreadPool::ThreadPool(std::shared_ptr<TaskQueue> taskQueue)
    : mTaskQueue(taskQueue)
    , mStop(false)
    , mRemoveCount(0)
{
    if (mTaskQueue == nullptr)
    {
        return;
    }

    uint64_t poolSize = std::thread::hardware_concurrency() + 1;

    while (poolSize--)
    {
        AddThread();
    }
}

ThreadPool::ThreadPool(std::shared_ptr<TaskQueue> taskQueue, uint64_t poolSize)
    : mTaskQueue(taskQueue)
    , mStop(false)
    , mRemoveCount(0)
{
    if (mTaskQueue == nullptr)
    {
        return;
    }

    while (poolSize--)
    {
        AddThread();
    }
}

ThreadPool::~ThreadPool()
{
    Stop();
}

uint64_t ThreadPool::GetPoolSize() const
{
    std::lock_guard<decltype(mLock)> scopeLock(mLock);
    return mWorkerThreads.size();
}

void ThreadPool::AddThread()
{
    std::lock_guard<decltype(mLock)> scopeLock(mLock);
    auto thread = std::make_shared<std::thread>(&ThreadPool::TaskLoop, this);
    mWorkerThreads[thread->get_id()] = thread;
}

void ThreadPool::RemoveThread()
{
    std::lock_guard<decltype(mLock)> scopeLock(mLock);
    if (mWorkerThreads.size() > mRemoveCount)
    {
        mRemoveCount++;
    }
}

void ThreadPool::Stop()
{
    mStop = true;

    // Join all threads
    for (auto& thread : mWorkerThreads)
    {
        thread.second->join();
    }

    std::lock_guard<decltype(mLock)> scopeLock(mLock);
    mWorkerThreads.clear();
}

void ThreadPool::TaskLoop()
{
    while (!mStop)
    {
        if (mRemoveCount)
        {
            std::lock_guard<decltype(mLock)> scopeLock(mLock);
            mWorkerThreads.erase(std::this_thread::get_id());
            mRemoveCount--;
            break;
        }

        TaskPtr task = nullptr;
        if (GetPoolSize() == 0)
        {
            task = mTaskQueue->GetTask(std::chrono::milliseconds(0));
        }
        else
        {
            task = mTaskQueue->GetTask(std::chrono::milliseconds(30));
        }

        if (task != nullptr)
        {
            mPreprocess(task);
            task->Execute();
            mPostprocess(task);
            mTaskQueue->CompleteTask(task);
        }
    }
}
