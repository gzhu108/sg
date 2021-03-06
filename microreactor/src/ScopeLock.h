#pragma once
#ifndef microreactor_ScopeLock
#define microreactor_ScopeLock

#include <mutex>
#include <atomic>


namespace microreactor
{
    namespace TaskManagerSingleton
    { 
        extern uint64_t GetThreadPoolSize();
    }

    template<class Mutex>
    class ScopeLock
    {
    public:
        typedef Mutex mutex_type;

        explicit ScopeLock(Mutex& m)
            : mMutex(m)
            , mShouldLock(TaskManagerSingleton::GetThreadPoolSize() != 0)
        {
            if (mShouldLock)
            {
                mMutex.lock();
            }
        }

        ~ScopeLock()
        {
            if (mShouldLock)
            {
                mMutex.unlock();
            }
        }

        ScopeLock(const ScopeLock&) = delete;
        ScopeLock& operator=(const ScopeLock&) = delete;

    private:
        Mutex& mMutex;
        std::atomic<bool> mShouldLock;
    };
}

#endif // microreactor_ScopeLock
