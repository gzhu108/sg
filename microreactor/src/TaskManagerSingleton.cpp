#include "TaskManagerSingleton.h"

using namespace sg::microreactor;


static std::recursive_mutex gTaskManagerLock;
static std::shared_ptr<TaskManager> gTaskManagerInstance;

uint64_t TaskManagerSingleton::GetThreadPoolSize()
{
	return GetTaskManager()->GetThreadPoolSize();
}

void TaskManagerSingleton::SetThreadPoolSize(const uint64_t& threadPoolSize)
{
    std::lock_guard<decltype(gTaskManagerLock)> scopeLock(gTaskManagerLock);

    if (gTaskManagerInstance == nullptr)
    {
        gTaskManagerInstance = std::make_shared<TaskManager>(threadPoolSize);
    }
    else
    {
        gTaskManagerInstance->SetThreadPoolSize(threadPoolSize);
    }
}

std::shared_ptr<TaskManager> TaskManagerSingleton::GetTaskManager()
{
    std::lock_guard<decltype(gTaskManagerLock)> scopeLock(gTaskManagerLock);

    if (gTaskManagerInstance == nullptr)
    {
        gTaskManagerInstance = std::make_shared<TaskManager>(0);
    }
    
    return gTaskManagerInstance;
}

void TaskManagerSingleton::DestroyTaskManager()
{
    std::lock_guard<decltype(gTaskManagerLock)> scopeLock(gTaskManagerLock);

    if (gTaskManagerInstance != nullptr)
    {
        gTaskManagerInstance->Stop();
        gTaskManagerInstance = nullptr;
    }
}
