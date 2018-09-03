#pragma warning(disable : 4512)

#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <functional>
#include <vector>
#include "NetworkUtility.h"
#include "WorldService.h"
#include "ConfigurationXml.h"

using namespace sg::microreactor;
using namespace worldserver;


namespace
{
    class ConsoleLogger
    {
    public:
        void Log(const std::string& text)
        {
            printf("%s\n", text.c_str());
        }
    };
}

volatile bool terminateSignal = false;
void SingalHandler(int type)
{
    terminateSignal = true;
    STOP_TASK_MANAGER();
}

int32_t main(int32_t argc, const char* argv[])
{
    ConsoleLogger logger;
    GET_LOGGER().AddLogger(std::bind(&ConsoleLogger::Log, &logger, std::placeholders::_1));

    LOG("World Server Started");
    LOG("press ctrl+c to terminate");

    // Set signal handlers for graceful termination
    signal(SIGABRT, SingalHandler);
    signal(SIGINT, SingalHandler);
    signal(SIGTERM, SingalHandler);

#ifndef _MSC_VER
    signal(SIGPIPE, SIG_IGN);
#endif

    std::string configFilePath;
    const char* configArgs[] = {"--config", "-c"};
    
    for (int32_t i = 1; i < argc; i++)
    {
        if (FIND_CMD(configArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            configFilePath = argv[++i];
        }
        else
        {
            LOG("Unknown command line argument: %s", argv[i]);
        }
    }
    
    LOG("Configuration file: %s", configFilePath.c_str());
    auto configuration = std::make_shared<ConfigurationXml>(configFilePath, "Service");
    ConfigurationSingleton::InitializeConfiguration(configuration);

    if (configuration != nullptr)
    {
        // Get the thread pool size from the configuration file
        uint64_t threadPoolSize = 0;
        configuration->GetValue("ThreadPoolSize", threadPoolSize);
        TaskManagerSingleton::SetThreadPoolSize(threadPoolSize);
    }

    WorldService service;
    if (service.Start())
    {
        // Create StreetGangService
        START_BLOCKING_TASK_LOOP();

        // Stop service
        service.Stop();
    }
    else
    {
        LOG("Failed to start the worldserver");
    }

    // Cancell all tasks
    uint64_t cancelCount = CANCEL_TASKS();
    if (cancelCount > 0)
    {
        while (GET_ACTIVE_TASK_COUNT() > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    LOG("All done...");
    return 0;
}
