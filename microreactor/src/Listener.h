#pragma once
#ifndef sg_microreactor_Listener
#define sg_microreactor_Listener

#include <atomic>
#include "Profile.h"
#include "Endpoint.h"


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
        virtual std::shared_ptr<Endpoint> GetEndpoint() { return mEndpoint; }

        virtual bool Initialize(std::shared_ptr<Endpoint> endpoint);
        
    protected:
        std::atomic<Status> mStatus;
        std::shared_ptr<Endpoint> mEndpoint;
    };
}}


#endif // sg_microreactor_Listener
