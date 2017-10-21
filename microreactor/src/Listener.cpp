#include "Listener.h"

using namespace sg::microreactor;


Listener::Listener()
    : mStatus(Status::Unknown)
{
}

Listener::~Listener()
{
    Stop();
    mHost = nullptr;
}

bool Listener::Start()
{
    if (mStatus != Status::Initialized && mStatus != Status::Stopped)
    {
        // Listener not initialized
        return false;
    }
    
    mStatus = Status::Started;
    
    // Start listen on the host
    return mHost->Start();
}

bool Listener::Stop()
{
    if (mStatus != Status::Started)
    {
        // Listener not started
        return false;
    }

    if (mHost->Stop())
    {
        mStatus = Status::Stopped;
        return true;
    }

    return false;
}

bool Listener::Initialize(std::shared_ptr<Host> host)
{
    mHost = host;
    if (mHost != nullptr && !mHost->IsClosed())
    {
        mStatus = Status::Initialized;
        return true;
    }

    return false;
}
