#pragma once
#ifndef worldapi_WorldCache
#define worldapi_WorldCache

#include <mutex>
#include <unordered_map>
#include "World.h"


namespace worldapi
{
    class WorldCache
    {
    public:
        WorldCache();
        virtual ~WorldCache();

    public:
        bool GetWorldId(const std::string& worldName, WorldId& worldId) const;
        bool GetWorld(const WorldId& worldId, World& world) const;

        void AddWorldId(const std::string& worldName, const WorldId& worldId);
        void AddWorld(const WorldId& worldId, const World& world);

    protected:
        mutable std::recursive_mutex mLock;
        std::unordered_map<std::string, WorldId> mWorldIdMap;
        std::unordered_map<WorldId, World> mWorldCache;
   };
}

#endif // worldapi_WorldCache
