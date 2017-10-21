#pragma once
#ifndef sg_microreactor_Listener
#define sg_microreactor_Listener

#include <atomic>
#include "Profile.h"
#include "Host.h"


namespace sg { namespace microreactor
{
    class Listener
    {
    public:
        Listener();
        virtual ~Listener();

    public:
        enum class Status
        {
            Unknown,
            Initialized,
            Started,
            Stopped,
        };

        virtual bool Start();
        virtual bool Stop();

        virtual Status GetStatus() { return mStatus; }
        virtual std::shared_ptr<Host> GetHost() { return mHost; }

        virtual bool Initialize(std::shared_ptr<Host> host);
        
    protected:
        std::atomic<Status> mStatus;
        std::shared_ptr<Host> mHost;
    };
}}


#endif // sg_microreactor_Listener
