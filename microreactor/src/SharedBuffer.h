#pragma once
#ifndef microreactor_SharedBuffer
#define microreactor_SharedBuffer

#include <string>
#include <memory>
#include "Property.h"


namespace microreactor
{
    typedef std::string Buffer;
    
    class SharedBuffer : public std::shared_ptr<Buffer>
    {
    public:
        SharedBuffer();
        SharedBuffer(const SharedBuffer &sharedBuffer);
        SharedBuffer(std::shared_ptr<Buffer> sharedBuffer);
        SharedBuffer(Buffer *sharedBuffer);
        virtual ~SharedBuffer();

        PROPERTY(Timeout, std::chrono::milliseconds, std::chrono::milliseconds(100));
        Signal<std::chrono::milliseconds>& TimedOut = mTimedOut;

        SharedBuffer &operator=(const SharedBuffer &sharedBuffer);

    public:
        virtual void StartTimer();
        virtual bool HasTimedOut();

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> mTimer { std::chrono::high_resolution_clock::now() };
        Emittable<std::chrono::milliseconds> mTimedOut;
    };
}


#endif // microreactor_SharedBuffer
