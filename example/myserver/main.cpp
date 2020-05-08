#pragma warning(disable : 4512)

#include <signal.h>
#include <stdlib.h>
#include "Microreactor.h"
#include "MyService.h"

using namespace sg::microreactor;
using namespace myserver;


int32_t main(int32_t argc, const char* argv[])
{
    GET_LOGGER().AddLogger([](const std::string& text)
    {
        printf("%s\n", text.c_str());
    });

    LOG("myserver (%s)", sg::microreactor::StringUtility::GetHttpTimeString().c_str());
    LOG("press ctrl+c to terminate");

    std::string configFilePath;
    std::string hostName;
    uint16_t hostPort = 0;
    uint16_t securePort = 0;

    const char* configArgs[] = {"--config", "-c"};
    const char* hostArgs[] = {"--host", "-h"};
    const char* portArgs[] = {"--port", "-p"};
    
    for (int32_t i = 1; i < argc; i++)
    {
        if (FIND_CMD(configArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            configFilePath = argv[++i];
        }
        else if (FIND_CMD(hostArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            hostName = argv[++i];
        }
        else if (FIND_CMD(portArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            hostPort = std::atoi(argv[++i]);
        }
        else
        {
            LOG("Unknown command line argument: %s", argv[i]);
        }
    }
    
    LOG("Configuration file: %s", configFilePath.c_str());
        
    // Create metricator service profile
    auto configuration = std::make_shared<ConfigurationXml>(configFilePath, "Service");
    if (hostName.empty())
    {
        configuration->GetValue("Address", hostName);
    }
    if (hostPort == 0)
    {
        configuration->GetValue("Port", hostPort);
    }
    if (securePort == 0)
    {
        configuration->GetValue("SecurePort", securePort);
    }

    ConfigurationSingleton::InitializeConfiguration(configuration);

    // Create the simple REST service
    auto simpleProfile = std::make_shared<Profile>();
    simpleProfile->Address.set(hostName);
    simpleProfile->Port.set(hostPort);

    // Start the REST services
    if (!Application::Context().Run(std::make_shared<MyService>(simpleProfile)))
    {
        LOG("Failed to start the myserver");
    }

    LOG("All done...");
    return 0;
}
