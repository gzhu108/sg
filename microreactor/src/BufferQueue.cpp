#include "BufferQueue.h"
#include "ScopeLock.h"

using namespace microreactor;


BufferQueue::BufferQueue()
{
}

BufferQueue::~BufferQueue()
{
    ClearQueue();
}

void BufferQueue::ClearQueue()
{
    ScopeLock<decltype(mBufferQueueMutex)> scopeLock(mBufferQueueMutex);
    mBufferQueue.clear();
}

uint64_t BufferQueue::GetQueueDepth()
{
    ScopeLock<decltype(mBufferQueueMutex)> scopeLock(mBufferQueueMutex);
    return mBufferQueue.size();
}

bool BufferQueue::Submit(SharedBuffer buffer)
{
    if (buffer->empty())
    {
        return false;
    }

    ScopeLock<decltype(mBufferQueueMutex)> scopeLock(mBufferQueueMutex);
    mBufferQueue.emplace_back(buffer);
    return !mBufferQueue.empty();
}

bool BufferQueue::PutBack(SharedBuffer buffer)
{
    if (buffer->empty())
    {
        return false;
    }
    
    ScopeLock<decltype(mBufferQueueMutex)> scopeLock(mBufferQueueMutex);
    mBufferQueue.emplace_front(buffer);
    return !mBufferQueue.empty();
}

SharedBuffer BufferQueue::GetBuffer()
{
    ScopeLock<decltype(mBufferQueueMutex)> scopeLock(mBufferQueueMutex);

    if (mBufferQueue.empty())
    {
        return nullptr;
    }

    SharedBuffer buffer = mBufferQueue.front();
    mBufferQueue.pop_front();
    return buffer;
}
