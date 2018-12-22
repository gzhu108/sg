#pragma once
#ifndef sg_microreactor_TaskQueue
#define sg_microreactor_TaskQueue

#include <list>
#include <set>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include "Task.h"


namespace sg { namespace microreactor
{
    class TaskQueue
    {
    public:
        TaskQueue();
        virtual ~TaskQueue();
        
    public:
        virtual void Start();
        virtual void Stop();

        // This operation is thread safe.
        virtual void ClearQueue();

        // This operation is thread safe.
        virtual uint64_t GetQueueDepth();

        // This operation does not obtain a mutex lock.
        // Thus, this operation is not thread safe.
        virtual uint64_t GetTaskQueueDepth();
        
        // This operation does not obtain a mutex lock.
        // Thus, this operation is not thread safe.
        virtual bool EnqueueTask(TaskPtr task);
        
        // This operation does not obtain a mutex lock.
        // Thus, this operation is not thread safe.
        virtual TaskPtr DequeueTask();
        
        // This operation is thread safe.
        virtual bool Submit(TaskPtr task);
        
        // This operation is thread safe.
        virtual TaskPtr GetTask(const std::chrono::milliseconds& timeout);

        virtual uint64_t GetActiveTaskCount();
        virtual uint64_t CancelTasks();
        virtual void CompleteTask(TaskPtr task);

    protected:
        std::mutex mTaskQueueMutex;
        std::list<TaskPtr> mTaskQueue;
        std::set<TaskPtr> mActivePool;
        std::condition_variable mTaskSubmitted;
        std::atomic<bool> mStop { false };
    };
}}


#endif // sg_microreactor_TaskQueue
