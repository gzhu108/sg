#pragma once
#ifndef microreactor_ThreadPool
#define microreactor_ThreadPool

#include <map>
#include <atomic>
#include <thread>
#include "TaskProcessHook.h"
#include "TaskQueue.h"


namespace microreactor
{
    class ThreadPool : public TaskProcessHook
    {
    public:
        explicit ThreadPool(std::shared_ptr<TaskQueue> taskQueue);
        ThreadPool(std::shared_ptr<TaskQueue> taskQueue, uint64_t poolSize);
        virtual ~ThreadPool();
        
    public:
        uint64_t GetPoolSize() const;
        void AddThread();
        void RemoveThread();
        void Stop();
        void TaskLoop();
        
    protected:
        mutable std::recursive_mutex mLock;
        std::map<std::thread::id, std::shared_ptr<std::thread>> mWorkerThreads;
        std::shared_ptr<TaskQueue> mTaskQueue;
        std::atomic<uint64_t> mRemoveCount;
        std::atomic<bool> mStop;
    };
}


#endif // microreactor_ThreadPool
