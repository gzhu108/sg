#include "StreetGangSessionManager.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


std::shared_ptr<StreetGangSessionManager> StreetGangSessionManager::mInstance;


StreetGangSessionManager& StreetGangSessionManager::GetInstance()
{
    if (mInstance == nullptr)
    {
        mInstance.reset(new StreetGangSessionManager());
    }

    return *mInstance;
}

StreetGangSessionManager::StreetGangSessionManager()
{
}

StreetGangSessionManager::~StreetGangSessionManager()
{
}

const SessionId& StreetGangSessionManager::CreateWorld(const std::string& worldName)
{
    auto sessionItr = mNamedSessions.find(worldName);
    if (sessionItr != mNamedSessions.end())
    {
        return sessionItr->second->Id.cref();
    }
    
    auto session = std::make_shared<WorldServiceProvider>();
    session->Id.set((SessionId)session.get());
    mNamedSessions[worldName] = session;
    AddSession(session);
    
    return session->Id.cref();
}
