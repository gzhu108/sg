#include "WorldCache.h"

using namespace sg::microreactor;
using namespace worldapi;


WorldCache::WorldCache()
{
}

WorldCache::~WorldCache()
{
}

bool WorldCache::GetWorldId(const std::string& worldName, WorldId& worldId) const
{
    sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);

    auto found = mWorldIdMap.find(worldName);
    if (found == mWorldIdMap.end())
    {
        return false;
    }

    worldId = found->second;
    return true;
}

bool WorldCache::GetWorld(const WorldId& worldId, World& world) const
{
    sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);

    auto found = mWorldCache.find(worldId);
    if (found == mWorldCache.end())
    {
        return false;
    }

    world = found->second;
    return true;
}

void WorldCache::AddWorldId(const std::string& worldName, const WorldId& worldId)
{
    sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);
    mWorldIdMap[worldName] = worldId;
}

void WorldCache::AddWorld(const WorldId& worldId, const World& world)
{
    sg::microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);
    mWorldCache[worldId] = world;
}
