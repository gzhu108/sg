#pragma once
#ifndef sg_microreactor_TaskCategorizedQueue
#define sg_microreactor_TaskCategorizedQueue

#include "TaskQueue.h"


namespace sg { namespace microreactor
{
    class TaskCategorizedQueue : public TaskQueue
    {
    public:
        TaskCategorizedQueue();
        virtual ~TaskCategorizedQueue();
        
    public:
        // This operation is thread safe.
        virtual void ClearQueue() override;

        // This operation does not obtain a mutex lock.
        // Thus, this operation is not thread safe.
        virtual uint64_t GetTaskQueueDepth() override;
        
        // This operation does not obtain a mutex lock.
        // Thus, this operation is not thread safe.
        virtual bool EnqueueTask(TaskPtr task, int64_t category);
        
        // This operation does not obtain a mutex lock.
        // Thus, this operation is not thread safe.
        virtual TaskPtr DequeueTask() override;
        
        // This operation is thread safe.
        // category can be typeid(*this).hash_code()
        virtual bool Submit(TaskPtr task, int64_t category = 0);
        
        virtual uint64_t CancelTasks() override;

    protected:
        typedef std::map<int64_t, std::list<TaskPtr>> CategoryQueue;
        typedef std::list<std::list<TaskPtr>*> Multiqueue;

        CategoryQueue mCategoryQueue;
        Multiqueue mTaskCategorizedQueue;
    };
}}


#endif // sg_microreactor_TaskCategorizedQueue
