#include "SharedBuffer.h"

using namespace microreactor;


SharedBuffer::SharedBuffer()
{
}

SharedBuffer::SharedBuffer(const SharedBuffer &sharedBuffer)
    : std::shared_ptr<Buffer>(sharedBuffer)
{
    Timeout.set(sharedBuffer.Timeout.cref());
}

SharedBuffer::SharedBuffer(std::shared_ptr<Buffer> sharedBuffer)
    : std::shared_ptr<Buffer>(sharedBuffer)
{
}

SharedBuffer::SharedBuffer(Buffer* sharedBuffer)
    : std::shared_ptr<Buffer>(sharedBuffer)
{
}

SharedBuffer::~SharedBuffer()
{
}

SharedBuffer& SharedBuffer::operator=(const SharedBuffer &sharedBuffer)
{
    std::shared_ptr<Buffer>::operator=(sharedBuffer);
    Timeout.set(sharedBuffer.Timeout.cref());
    return *this;
}

void SharedBuffer::StartTimer()
{
    mTimer = std::chrono::high_resolution_clock::now();
}

bool SharedBuffer::HasTimedOut()
{
    if (Timeout() > std::chrono::milliseconds::zero())
    {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mTimer);
        if (duration >= Timeout())
        {
            mTimedOut(duration);
            return true;
        }
    }

    return false;
}
