#pragma once
#ifndef sg_microreactor_TaskManager
#define sg_microreactor_TaskManager

#include <functional>
#include "TaskProcessHook.h"
#include "TaskQueue.h"
#include "TaskFuture.h"


namespace sg { namespace microreactor
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
        virtual uint64_t CancelTasks(void* activeId = nullptr) { return mTaskQueue->CancelTasks(reinterpret_cast<uintptr_t>(activeId)); }
        virtual uint64_t GetActiveTaskCount(void* activeId = nullptr) { return mTaskQueue->GetActiveTaskCount(reinterpret_cast<uintptr_t>(activeId)); }
        virtual uint64_t GetQueueDepth() { return mTaskQueue->GetQueueDepth(); }

#if 0 && defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1) && (ATOMIC_INT_LOCK_FREE > 1)

        // Submit(1)
        template <typename TaskFunc>
        auto Submit(TaskFunc&& taskFunc, std::shared_ptr<Shareable> owner, void* activeId, const std::string& taskName = {}, bool insertBack = true) -> TaskFuture<decltype(taskFunc())>
        {
            typedef decltype(taskFunc()) ValueType;
            std::function<ValueType ()> run = std::bind(taskFunc);
            return Submit(run, owner, activeId, taskName, insertBack); // Call Submit(3)
        }

        // Submit(2)
        template <typename TaskFunc, typename ValueType>
        auto Submit(TaskFunc&& taskFunc, std::shared_ptr<Shareable> owner, void* activeId, const std::string& taskName = {}, bool insertBack = true) -> TaskFuture<decltype(taskFunc())>
        {
            //std::function<ValueType ()> run = std::bind(taskFunc);
            //return Submit(run, insertBack); // Call Submit(3)
            
            auto task = std::make_shared<PackagedTask<TaskFunc, ValueType>>(taskFunc);
            task->Owner.set(owner);
            task->Name.set(taskName);
            task->ActiveId.set(reinterpret_cast<uintptr_t>(activeId));
            TaskFuture<ValueType> future(task->GetFuture().share(), task);
            mTaskQueue->Submit(task, insertBack);
            return future;
        }
        
        // Submit(3)
        template <typename ValueType>
        auto Submit(std::function<ValueType ()> run, std::shared_ptr<Shareable> owner, void* activeId, const std::string& taskName = {}, bool insertBack = true) -> TaskFuture<ValueType>
        {
            auto task = std::make_shared<PromisedTask<ValueType>>(run);
            task->Owner.set(owner);
            task->Name.set(taskName);
            task->ActiveId.set(reinterpret_cast<uintptr_t>(activeId));
            TaskFuture<ValueType> future(task->GetFuture().share(), task);
            mTaskQueue->Submit(task, insertBack);
            return future;
        }

        // Submit(4)
        TaskFuture<void> Submit(BaseTaskPtr task, std::shared_ptr<Shareable> owner, void* activeId, const std::string& taskName = {}, bool insertBack = true)
        {
            task->Owner.set(owner);
            task->Name.set(taskName);
            task->ActiveId.set(reinterpret_cast<uintptr_t>(activeId));
            TaskFuture<void> future(task->GetFuture().share(), task);
            mTaskQueue->Submit(task, insertBack);
            return future;
        }

#else // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1) && (ATOMIC_INT_LOCK_FREE > 1)
        template <typename TaskFunc>
        bool Submit(TaskFunc&& taskFunc, std::shared_ptr<Shareable> owner, void* activeId, const std::string& taskName = {}, bool insertBack = true)
        {
            auto task = std::make_shared<Task>(taskFunc);
            return Submit(task, owner, activeId, taskName, insertBack);
        }

        inline bool Submit(std::function<void ()> run, std::shared_ptr<Shareable> owner, void* activeId, const std::string& taskName = {}, bool insertBack = true)
        {
            auto task = std::make_shared<Task>(run);
            return Submit(task, owner, activeId, taskName, insertBack);
        }

#endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1) && (ATOMIC_INT_LOCK_FREE > 1)

        // Submit(5)
        inline bool Submit(TaskPtr task, std::shared_ptr<Shareable> owner, void* activeId, const std::string& taskName = {}, bool insertBack = true)
        {
            task->Owner.set(owner);
            task->Name.set(taskName);
            task->ActiveId.set(reinterpret_cast<uintptr_t>(activeId));

            if (GetThreadPoolSize() == 0)
            {
                return mTaskQueue->EnqueueTask(task, insertBack);
            }

            return mTaskQueue->Submit(task, insertBack);
        }

    protected:
        //PROPERTY_GET_SET(ThreadPoolSize, uint64_t, 0, TaskManager::GetThreadPoolSize, TaskManager::SetThreadPoolSize);
        //virtual uint64_t GetThreadPoolSize(const PROPERTY_TYPE(ThreadPoolSize)& propertyValue);
        //virtual void SetThreadPoolSize(PROPERTY_TYPE(ThreadPoolSize)& propertyValue, const uint64_t& threadPoolSize);
        
    protected:
        std::shared_ptr<TaskQueue> mTaskQueue;
        std::shared_ptr<ThreadPool> mThreadPool;
    };
}}


#endif // sg_microreactor_TaskManager
