#pragma once
#ifndef sg_microreactor_ConfigurationSingleton
#define sg_microreactor_ConfigurationSingleton

#include "Configuration.h"


namespace sg { namespace microreactor
{
    namespace ConfigurationSingleton
    {
        void InitializeConfiguration(std::shared_ptr<Configuration> configuration);
        std::shared_ptr<Configuration> GetConfiguration();
        void DestroyConfiguration();
    }
}}


#endif // sg_microreactor_ConfigurationSingleton
