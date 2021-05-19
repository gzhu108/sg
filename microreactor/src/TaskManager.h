#pragma once
#ifndef microreactor_TaskManager
#define microreactor_TaskManager

#include <functional>
#include "TaskProcessHook.h"
#include "TaskQueue.h"
#include "TaskFuture.h"


namespace microreactor
{
    class TaskProcessHook;
    class ThreadPool;
    
    class TaskManager : public TaskProcessHook
    {
    public:
        TaskManager();
        explicit TaskManager(uint64_t threadPoolSize);
        virtual ~TaskManager();
        
    public:
        virtual uint64_t GetThreadPoolSize();
        virtual void SetThreadPoolSize(const uint64_t& threadPoolSize);

        virtual void BlockingTaskLoop();
        void Start(uint64_t threadPoolSize);
        uint64_t Stop();

        virtual void Pause();
        virtual void Resume();

        virtual TaskProcessHook& GetTaskProcessHook();
        virtual uint64_t CancelTasks() { return mTaskQueue->CancelTasks(); }
        virtual uint64_t GetActiveTaskCount() { return mTaskQueue->GetActiveTaskCount(); }
        virtual uint64_t GetQueueDepth() { return mTaskQueue->GetQueueDepth(); }

        template <typename TaskFunc>
        TaskPtr CreateTask(TaskFunc&& taskFunc, const std::string& taskName = {})
        {
            auto task = std::make_shared<Task>(taskFunc);
            task->Name(taskName);
            return task;
        }

        template <typename TaskFunc>
        TaskPtr CreateTask(std::function<void ()> run, const std::string& taskName = {})
        {
            auto task = std::make_shared<Task>(run);
            task->Name(taskName);
            return task;
        }

        // Submit(5)
        inline TaskPtr Submit(TaskPtr task)
        {
            if (GetThreadPoolSize() == 0)
            {
                if (mTaskQueue->EnqueueTask(task))
                {
                    return task;
                }
            }
            else if (mTaskQueue->Submit(task))
            {
                return task;
            }

            return nullptr;
        }
       
    protected:
        std::shared_ptr<TaskQueue> mTaskQueue;
        std::shared_ptr<ThreadPool> mThreadPool;
    };
}


#endif // microreactor_TaskManager
