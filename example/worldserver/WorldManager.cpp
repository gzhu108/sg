#include "WorldManager.h"

using namespace microreactor;
using namespace worldapi;
using namespace worldserver;


std::shared_ptr<WorldManagerSingleton> WorldManagerSingleton::mInstance;


WorldManagerSingleton& WorldManagerSingleton::GetInstance()
{
    if (mInstance == nullptr)
    {
        mInstance.reset(new WorldManagerSingleton());
    }

    return *mInstance;
}

WorldManagerSingleton::WorldManagerSingleton()
{
}

WorldManagerSingleton::~WorldManagerSingleton()
{
}

const WorldId& WorldManagerSingleton::CreateWorld(const std::string& worldName)
{
    microreactor::ScopeLock<decltype(mLock)> scopeLock(mLock);

    auto worldItr = mNamedWorlds.find(worldName);
    if (worldItr != mNamedWorlds.end())
    {
        return worldItr->second->mId;
    }
    
    auto world = std::make_shared<WorldProvider>();
    world->mId = (WorldId)world.get();
    mNamedWorlds[worldName] = world;
    AddWorld(world);
    
    return world->mId;
}
