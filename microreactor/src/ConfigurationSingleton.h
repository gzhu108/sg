#pragma once
#ifndef microreactor_ConfigurationSingleton
#define microreactor_ConfigurationSingleton

#include "Configuration.h"


namespace microreactor
{
    namespace ConfigurationSingleton
    {
        void InitializeConfiguration(std::shared_ptr<Configuration> configuration);
        std::shared_ptr<Configuration> GetConfiguration();
        void DestroyConfiguration();
    }
}


#endif // microreactor_ConfigurationSingleton
