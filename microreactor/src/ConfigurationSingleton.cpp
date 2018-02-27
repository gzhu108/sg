#include "ConfigurationSingleton.h"

using namespace sg::microreactor;


static std::recursive_mutex gConfigurationLock;
static std::shared_ptr<Configuration> gConfigurationInstance;

void ConfigurationSingleton::InitializeConfiguration(std::shared_ptr<Configuration> configuration)
{
    std::lock_guard<decltype(gConfigurationLock)> scopeLock(gConfigurationLock);
    gConfigurationInstance = configuration;
}

std::shared_ptr<Configuration> ConfigurationSingleton::GetConfiguration()
{
    std::lock_guard<decltype(gConfigurationLock)> scopeLock(gConfigurationLock);
    return gConfigurationInstance;
}

void ConfigurationSingleton::DestroyConfiguration()
{
    std::lock_guard<decltype(gConfigurationLock)> scopeLock(gConfigurationLock);
    gConfigurationInstance = nullptr;
}
