#pragma once
#ifndef sg_microreactor_ScopeLock
#define sg_microreactor_ScopeLock

#include <mutex>


namespace sg { namespace microreactor
{
    namespace TaskManagerSingleton
    { 
        extern uint64_t GetThreadPoolSize();
    }

    template<class _Mutex>
    class ScopeLock
    {
    public:
        typedef _Mutex mutex_type;

        explicit ScopeLock(_Mutex& _Mtx)
            : mMutex(_Mtx)
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
        _Mutex& mMutex;
        bool mShouldLock;
    };
}}

#endif // sg_microreactor_ScopeLock
