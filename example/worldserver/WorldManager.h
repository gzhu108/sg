#pragma once
#ifndef worldserver_WorldManager
#define worldserver_WorldManager

#include <unordered_map>
#include "WorldProvider.h"


namespace worldserver
{
    template<typename WorldType>
    class WorldManager
    {
    public:
        worldapi::WorldId AddWorld(std::shared_ptr<WorldType> world)
        {
            sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);
            mWorldCollection[world->mId] = world;
            return world->mId;
        }

        bool RemoveWorld(const worldapi::WorldId& worldId)
        {
            sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);
            return mWorldCollection.erase(worldId) != 0;
        }

        std::shared_ptr<WorldType> GetWorld(const worldapi::WorldId& worldId)
        {
            sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);
            auto found = mWorldCollection.find(worldId);
            if (found == mWorldCollection.end())
            {
                return nullptr;
            }

            return found->second;
        }

    protected:
        std::recursive_mutex mLock;
        std::unordered_map<worldapi::WorldId, std::shared_ptr<WorldType>> mWorldCollection;
    };

    class WorldManagerSingleton : public WorldManager<WorldProvider>
    {
    public:
        virtual ~WorldManagerSingleton();
        
    public:
        static WorldManagerSingleton& GetInstance();
        const worldapi::WorldId& CreateWorld(const std::string& worldName);

    protected:
        WorldManagerSingleton();
        static std::shared_ptr<WorldManagerSingleton> mInstance;
    
    protected:
        std::unordered_map<std::string, std::shared_ptr<WorldProvider>> mNamedWorlds;
    };
}

#endif // worldserver_WorldManager
