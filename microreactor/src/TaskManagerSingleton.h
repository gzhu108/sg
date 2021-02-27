#pragma once
#ifndef microreactor_TaskManagerSingleton
#define microreactor_TaskManagerSingleton

#include "TaskManager.h"

#define START_BLOCKING_TASK_LOOP() microreactor::TaskManagerSingleton::GetTaskManager()->BlockingTaskLoop()
#define STOP_TASK_MANAGER() microreactor::TaskManagerSingleton::GetTaskManager()->Stop()
#define GET_TASK_PROCESS_HOOK() microreactor::TaskManagerSingleton::GetTaskManager()->GetTaskProcessHook()
#define GET_ACTIVE_TASK_COUNT(...) microreactor::TaskManagerSingleton::GetTaskManager()->GetActiveTaskCount(__VA_ARGS__)
#define CANCEL_TASKS(...) microreactor::TaskManagerSingleton::GetTaskManager()->CancelTasks(__VA_ARGS__)
#define DESTROY_TASK_MANAGER() microreactor::TaskManagerSingleton::DestroyTaskManager()
#define CREATE_TASK(...) microreactor::TaskManagerSingleton::GetTaskManager()->CreateTask(__VA_ARGS__)
#define SUBMIT_TASK(...) microreactor::TaskManagerSingleton::GetTaskManager()->Submit(__VA_ARGS__)
#define SUBMIT(...) SUBMIT_TASK(CREATE_TASK(__VA_ARGS__))
#define SUBMIT_MEMBER(_member, ...) AddTask(SUBMIT(std::bind(&_member, this), __VA_ARGS__))


#define CANCEL_ALL_TASKS_AND_DESTROY_TASK_MANAGER() \
    microreactor::TaskManagerSingleton::GetTaskManager()->Pause(); \
    uint64_t cancelCount = CANCEL_TASKS(); \
    if (cancelCount > 0) \
    { \
        while (GET_ACTIVE_TASK_COUNT() > 0) \
        { \
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); \
        } \
    } \
    DESTROY_TASK_MANAGER()


namespace microreactor
{
    namespace TaskManagerSingleton
    {
        uint64_t GetThreadPoolSize();
        void SetThreadPoolSize(const uint64_t& threadPoolSize);
        std::shared_ptr<TaskManager> GetTaskManager();
        void DestroyTaskManager();
    }
}


#endif // microreactor_TaskManagerSingleton
