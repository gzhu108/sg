#pragma once
#ifndef streetgangserver_StreetGangSessionManager
#define streetgangserver_StreetGangSessionManager

#include <unordered_map>
#include "WorldServiceProvider.h"


namespace streetgangserver
{
    template<typename SessionType>
    class SessionManager
    {
    public:
        typedef typename SessionType::SessionId SessionId;

        SessionId AddSession(std::shared_ptr<SessionType> session)
        {
            sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);
            mSessionCollection[session->Id.cref()] = session;
            return session->Id.cref();
        }

        bool RemoveSession(const SessionId& sessionId)
        {
            sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);
            return mSessionCollection.erase(sessionId) != 0;
        }

        std::shared_ptr<SessionType> GetSession(const SessionId& sessionId)
        {
            sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);
            auto found = mSessionCollection.find(sessionId);
            if (found == mSessionCollection.end())
            {
                return nullptr;
            }

            return found->second;
        }

    protected:
        std::recursive_mutex mLock;
        std::unordered_map<SessionId, std::shared_ptr<SessionType>> mSessionCollection;
    };

    class StreetGangSessionManager : public SessionManager<WorldServiceProvider>
    {
    public:
        virtual ~StreetGangSessionManager();
        
    public:
        static StreetGangSessionManager& GetInstance();
        const streetgangapi::SessionId& CreateWorld(const std::string& worldName);

    protected:
        StreetGangSessionManager();
        static std::shared_ptr<StreetGangSessionManager> mInstance;
    
    protected:
        std::unordered_map<std::string, std::shared_ptr<WorldServiceProvider>> mNamedSessions;
    };
}

#endif // streetgangserver_StreetGangSessionManager
