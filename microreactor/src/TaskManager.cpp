#include "TaskManager.h"
#include "ThreadPool.h"
#include "TaskQueue.h"

using namespace sg::microreactor;


TaskManager::TaskManager()
    : mTaskQueue(std::make_shared<TaskQueue>())
{
    mThreadPool = std::make_shared<ThreadPool>(mTaskQueue);
}

TaskManager::TaskManager(uint64_t threadPoolSize)
    : mTaskQueue(std::make_shared<TaskQueue>())
{
    mThreadPool = std::make_shared<ThreadPool>(mTaskQueue, threadPoolSize);
}

TaskManager::~TaskManager()
{
    Stop();
}

uint64_t TaskManager::GetThreadPoolSize()
{
    return mThreadPool->GetPoolSize();
}

void TaskManager::SetThreadPoolSize(const uint64_t& threadPoolSize)
{
    mThreadPool = std::make_shared<ThreadPool>(mTaskQueue, threadPoolSize);
}

void TaskManager::BlockingTaskLoop()
{
    if (mThreadPool != nullptr)
    {
        mThreadPool->TaskLoop();
    }
}

void TaskManager::Start(uint64_t threadPoolSize)
{
    mTaskQueue->Start();
    SetThreadPoolSize(threadPoolSize);
}

uint64_t TaskManager::Stop()
{
    mTaskQueue->Stop();
    mTaskQueue->ClearQueue();

    uint64_t threadPoolSize = 0;
    if (mThreadPool != nullptr)
    {
        threadPoolSize = mThreadPool->GetPoolSize();
        mThreadPool->Stop();
    }

    return threadPoolSize;
}

void TaskManager::Pause()
{
    mTaskQueue->Stop();
}

void TaskManager::Resume()
{
    mTaskQueue->Start();
}

TaskProcessHook& TaskManager::GetTaskProcessHook()
{
    return *std::static_pointer_cast<TaskProcessHook>(mThreadPool);
}
