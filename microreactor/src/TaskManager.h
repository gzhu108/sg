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
        template <typename TaskFunc, typename OwnerType>
        auto Submit(TaskFunc&& taskFunc, OwnerType owner, void* activeId, int64_t category = 0, const std::string& taskName = {}) -> TaskFuture<decltype(taskFunc())>
        {
            typedef decltype(taskFunc()) ValueType;
            std::function<ValueType ()> run = std::bind(taskFunc);
            return Submit(run, owner, activeId, category, taskName); // Call Submit(3)
        }

        // Submit(2)
        template <typename TaskFunc, typename ValueType, typename OwnerType>
        auto Submit(TaskFunc&& taskFunc, OwnerType owner, void* activeId, int64_t category = 0, const std::string& taskName = {}) -> TaskFuture<decltype(taskFunc())>
        {
            //std::function<ValueType ()> run = std::bind(taskFunc);
            //return Submit(run, category); // Call Submit(3)
            
            auto task = std::make_shared<PackagedTask<TaskFunc, ValueType>>(taskFunc);
            TaskFuture<ValueType> future(task->GetFuture().share(), task);
            Submit(task, owner, reinterpret_cast<uintptr_t>(activeId), category, taskName);
            return future;
        }
        
        // Submit(3)
        template <typename ValueType, typename OwnerType>
        auto Submit(std::function<ValueType ()> run, OwnerType owner, void* activeId, int64_t category = 0, const std::string& taskName = {}) -> TaskFuture<ValueType>
        {
            auto task = std::make_shared<PromisedTask<ValueType>>(run);
            TaskFuture<ValueType> future(task->GetFuture().share(), task);
            Submit(task, owner, reinterpret_cast<uintptr_t>(activeId), category, taskName);
            return future;
        }

        // Submit(4)
        template <typename ValueType, typename OwnerType>
        TaskFuture<void> Submit(BaseTaskPtr task, OwnerType owner, void* activeId, int64_t category = 0, const std::string& taskName = {})
        {
            TaskFuture<void> future(task->GetFuture().share(), task);
            Submit(task, owner, reinterpret_cast<uintptr_t>(activeId), category, taskName);
            return future;
        }

#else // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1) && (ATOMIC_INT_LOCK_FREE > 1)
        template <typename TaskFunc, typename OwnerType>
        bool Submit(TaskFunc&& taskFunc, OwnerType owner, void* activeId, int64_t category = 0, const std::string& taskName = {})
        {
            auto task = std::make_shared<Task>(taskFunc);
            return Submit(task, owner, reinterpret_cast<uintptr_t>(activeId), category, taskName);
        }

        template <typename TaskFunc, typename OwnerType>
        bool Submit(std::function<void ()> run, OwnerType owner, void* activeId, int64_t category = 0, const std::string& taskName = {})
        {
            auto task = std::make_shared<Task>(run);
            return Submit(task, owner, reinterpret_cast<uintptr_t>(activeId), category, taskName);
        }

#endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1) && (ATOMIC_INT_LOCK_FREE > 1)

        // Submit(5)
        template <typename OwnerType>
        bool Submit(TaskPtr task, OwnerType owner, uintptr_t activeId, int64_t category = 0, const std::string& taskName = {})
        {
            if (owner != nullptr)
            {
                task->Owner.set(owner->shared_from_this());
            }

            task->Name.set(taskName);
            task->ActiveId.set(activeId);
            return Submit(task, category);
        }

        // Submit(6)
        inline bool Submit(TaskPtr task, int64_t category = 0)
        {
            if (GetThreadPoolSize() == 0)
            {
                return mTaskQueue->EnqueueTask(task, category);
            }

            return mTaskQueue->Submit(task, category);
        }
       
    protected:
        std::shared_ptr<TaskQueue> mTaskQueue;
        std::shared_ptr<ThreadPool> mThreadPool;
    };
}}


#endif // sg_microreactor_TaskManager
