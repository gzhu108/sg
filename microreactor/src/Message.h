#pragma once
#ifndef sg_microreactor_Message
#define sg_microreactor_Message

#include <chrono>
#include "Common.h"


namespace sg { namespace microreactor
{
    class Message
    {
    public:
        Message();
        virtual ~Message();

        PROPERTY(TrackId, std::string);
        PROPERTY(ResponseTimeout, int64_t, 0);

    public:
        const std::chrono::time_point<std::chrono::high_resolution_clock>& GetCreationTime() { return mCreationTime; }
        const std::chrono::time_point<std::chrono::high_resolution_clock>& GetRequestTime() { return mRequestTime; }
        void SetRequestTime() { mRequestTime = std::chrono::high_resolution_clock::now(); }

        bool HasTimedOut();

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> mCreationTime{ std::chrono::high_resolution_clock::now() };
        std::chrono::time_point<std::chrono::high_resolution_clock> mRequestTime{ std::chrono::high_resolution_clock::now() };
    };
}}


#endif // sg_microreactor_Message
