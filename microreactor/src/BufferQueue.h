#pragma once
#ifndef microreactor_BufferQueue
#define microreactor_BufferQueue

#include <deque>
#include <mutex>
#include "SharedBuffer.h"


namespace microreactor
{
    class BufferQueue
    {
    public:
        BufferQueue();
        virtual ~BufferQueue();
        
    public:
        // This operation is thread safe.
        virtual void ClearQueue();

        // This operation is thread safe.
        virtual uint64_t GetQueueDepth();
        
        // This operation is thread safe.
        virtual bool Submit(SharedBuffer buffer);

        // This operation is thread safe.
        virtual bool PutBack(SharedBuffer buffer);
        
        // This operation is thread safe.
        virtual SharedBuffer GetBuffer();

    protected:
        std::mutex mBufferQueueMutex;
        std::deque<SharedBuffer> mBufferQueue;
    };
}


#endif // microreactor_BufferQueue
