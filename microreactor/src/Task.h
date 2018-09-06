#pragma once
#ifndef sg_microreactor_Task
#define sg_microreactor_Task

#include <functional>
#include <atomic>
#include <future>
#include <chrono>
#include "Shareable.h"
#include "Cancelable.h"


namespace sg { namespace microreactor
{
    enum class TaskStatus
    {
        Created = 0,
        Scheduled,
        Running,
        Completed,
        Canceled,
    };
    
    // Basic task object
    class Task : public Cancelable
    {
    public:
        Task();
        explicit Task(std::function<void ()> run);
        virtual ~Task();

        PROPERTY(Owner, std::shared_ptr<Shareable>);
        PROPERTY(Name, std::string);
        PROPERTY(ActiveId, uintptr_t, 0);

    public:
        virtual void SetExecuteTime(const std::chrono::time_point<std::chrono::high_resolution_clock>& executeTime);
        virtual TaskStatus GetStatus() const { return mStatus; }
        virtual void Schedule() { mStatus = TaskStatus::Scheduled; }
        virtual void Cancel() override { mStatus = TaskStatus::Canceled; }
        virtual bool Execute();
        
    protected:
        std::atomic<TaskStatus> mStatus{ TaskStatus::Created };
        std::chrono::time_point<std::chrono::high_resolution_clock> mExecuteTime{ std::chrono::high_resolution_clock::now() };
        std::function<void ()> mRun;
    };
    typedef std::shared_ptr<Task> TaskPtr;
    
#if 0 && defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1) && (ATOMIC_INT_LOCK_FREE > 1)

    // Promised task object
    template <typename ValueType>
    class PromisedTask : public Task
    {
    public:
        PromisedTask(std::function<ValueType ()> run)
        {
            mRun = [run, this]()
            {
                try
                {
                    RunTask(run, mPromise);
                }
                catch (...)
                {
                    mPromise.set_exception(std::current_exception());
                }
            };
        }
        
        std::future<ValueType> GetFuture()
        {
            return mPromise.get_future();
        }
        
    protected:
        void RunTask(std::function<ValueType ()> run, std::promise<ValueType>& promise)
        {
            promise.set_value(run());
        }
        
    protected:
        std::promise<ValueType> mPromise;
    };
    
    // Specialized template for 'void' functions.
    template<>
    inline void PromisedTask<void>::RunTask(std::function<void ()> run, std::promise<void>& promise)
    {
        run();
        promise.set_value();
    }
    
    // Packaged task object
    template <typename TaskFunc, typename ValueType>
    class PackagedTask : public Task
    {
    public:
        PackagedTask(TaskFunc&& taskFunc)
            : mPackage(taskFunc)
        {
            mRun = [this]()
            {
                mPackage();
            };
        }
        
        std::future<ValueType> GetFuture()
        {
            return mPackage.get_future();
        }
    
    protected:
        std::packaged_task<TaskFunc> mPackage;
    };
    
    // Base task object
    class BaseTask : public PromisedTask<void>
    {
    public:
        BaseTask() : PromisedTask<void>(std::bind(&BaseTask::Run, this)) {}
        virtual void Run() = 0;
    };
    typedef std::shared_ptr<BaseTask> BaseTaskPtr;

#else // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1) && (ATOMIC_INT_LOCK_FREE > 1)
    
    // Base task object
    class BaseTask : public Task
    {
    public:
        BaseTask() : Task(std::bind(&BaseTask::Run, this)) {}
        virtual void Run() = 0;
    };
    typedef std::shared_ptr<BaseTask> BaseTaskPtr;

#endif // defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1) && (ATOMIC_INT_LOCK_FREE > 1)
}}


#endif // sg_microreactor_Task
