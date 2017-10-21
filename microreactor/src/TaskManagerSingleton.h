#pragma once
#ifndef sg_microreactor_TaskManagerSingleton
#define sg_microreactor_TaskManagerSingleton

#include "TaskManager.h"

#define START_BLOCKING_TASK_LOOP() sg::microreactor::TaskManagerSingleton::GetTaskManager()->BlockingTaskLoop()
#define STOP_TASK_MANAGER() sg::microreactor::TaskManagerSingleton::GetTaskManager()->Stop()
#define SUBMIT(...) sg::microreactor::TaskManagerSingleton::GetTaskManager()->Submit(__VA_ARGS__)
#define GET_TASK_PROCESS_HOOK() sg::microreactor::TaskManagerSingleton::GetTaskManager()->GetTaskProcessHook()
#define GET_ACTIVE_TASK_COUNT(...) sg::microreactor::TaskManagerSingleton::GetTaskManager()->GetActiveTaskCount(__VA_ARGS__)
#define CANCEL_TASKS(...) sg::microreactor::TaskManagerSingleton::GetTaskManager()->CancelTasks(__VA_ARGS__)


namespace sg { namespace microreactor
{
    namespace TaskManagerSingleton
    {
	    uint64_t GetThreadPoolSize();
        void SetThreadPoolSize(const uint64_t& threadPoolSize);
        std::shared_ptr<TaskManager> GetTaskManager();
    }
}}


#endif // sg_microreactor_TaskManagerSingleton
