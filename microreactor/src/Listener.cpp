#include "Listener.h"

using namespace sg::microreactor;


Listener::Listener()
    : mStatus(Status::Unknown)
{
}

Listener::~Listener()
{
    Stop();
    mEndpoint = nullptr;
}

bool Listener::Start()
{
    if (mStatus != Status::Initialized && mStatus != Status::Stopped)
    {
        // Listener not initialized
        return false;
    }
    
    mStatus = Status::Started;
    
    // Start listening on the endpoint
    return mEndpoint->Start();
}

bool Listener::Stop()
{
    if (mStatus != Status::Started)
    {
        // Listener not started
        return false;
    }

    if (mEndpoint->Stop())
    {
        mStatus = Status::Stopped;
        return true;
    }

    return false;
}

bool Listener::Initialize(std::shared_ptr<Endpoint> endpoint)
{
    mEndpoint = endpoint;
    if (mEndpoint != nullptr && !mEndpoint->IsClosed())
    {
        mStatus = Status::Initialized;
        return true;
    }

    return false;
}
