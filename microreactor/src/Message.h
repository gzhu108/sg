#pragma once
#ifndef microreactor_Message
#define microreactor_Message

#include <chrono>
#include "Common.h"
#include "Parkable.h"


namespace microreactor
{
    class Reactor;

    class Message : public Parkable<std::string>
    {
    public:
        Message();
        virtual ~Message();

        PROPERTY(TrackId, std::string);
        PROPERTY(ResponseTimeout, int64_t, 0);
        PROPERTY(Client, std::shared_ptr<Reactor>);

    public:
        const std::chrono::time_point<std::chrono::high_resolution_clock>& GetCreationTime() { return mCreationTime; }
        const std::chrono::time_point<std::chrono::high_resolution_clock>& GetRequestTime() { return mRequestTime; }
        void SetRequestTime(std::chrono::time_point<std::chrono::high_resolution_clock> requestTime = std::chrono::high_resolution_clock::now()) { mRequestTime = requestTime; }

        bool HasTimedOut();

        virtual bool Read(const std::string& buffer);
        virtual bool Write(std::string& buffer) const;

        virtual bool Encode(std::ostream& stream) const;
        virtual bool Decode(std::istream& stream);

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> mCreationTime{ std::chrono::high_resolution_clock::now() };
        std::chrono::time_point<std::chrono::high_resolution_clock> mRequestTime{ std::chrono::high_resolution_clock::now() };
    };
}


#endif // microreactor_Message
