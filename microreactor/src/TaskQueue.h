#pragma once
#ifndef sg_microreactor_TaskQueue
#define sg_microreactor_TaskQueue

#include <list>
#include <map>
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
        virtual bool EnqueueTask(TaskPtr task, int64_t category);
        
        // This operation does not obtain a mutex lock.
        // Thus, this operation is not thread safe.
        virtual TaskPtr DequeueTask();
        
        // This operation is thread safe.
        virtual bool Submit(TaskPtr task, int64_t category = 0);
        
        // This operation is thread safe.
        virtual TaskPtr GetTask(const std::chrono::milliseconds& timeout);

        virtual uint64_t GetActiveTaskCount(uintptr_t owner);
        virtual uint64_t CancelTasks(uintptr_t activeId);
        virtual void CompleteTask(TaskPtr task);

    protected:
        typedef std::map<int64_t, std::list<TaskPtr>> CategoryQueue;
        typedef std::list<std::list<TaskPtr>*> Multiqueue;

        CategoryQueue mCategoryQueue;
        Multiqueue mTaskQueue;
        std::multimap<uintptr_t, TaskPtr> mActivePool;
        std::mutex mTaskQueueMutex;
        std::condition_variable mTaskSubmitted;
        std::atomic<bool> mStop { false };
    };
}}


#endif // sg_microreactor_TaskQueue
