#pragma once
#ifndef sg_microreactor_TaskFuture
#define sg_microreactor_TaskFuture

#include <future>
#include "Task.h"


namespace sg { namespace microreactor
{
    template <typename ValueType>
    class TaskFuture : public std::shared_future<ValueType>
    {
    public:
        TaskFuture(const std::shared_future<ValueType>& future, TaskPtr task)
            : std::shared_future<ValueType>(future)
            , mTask(task)
        {
        }

        virtual ~TaskFuture()
        {
        }

    public:
        TaskStatus GetTaskStatus() const
        {
            return mTask->GetStatus();
        }
        
    protected:
        TaskPtr mTask;
    };
}}


#endif // sg_microreactor_TaskFuture
