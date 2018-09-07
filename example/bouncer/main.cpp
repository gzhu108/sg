#pragma warning(disable : 4512)

#include <signal.h>
#include <cstring>
#include <stdlib.h>
#include <thread>
#include <functional>
#include "Microreactor.h"
#include "BouncerRestService.h"
#include "BouncerProfile.h"
#include "BouncerDispatcher.h"

using namespace sg::microreactor;
using namespace bouncer;


volatile bool terminateSignal = false;
void SingalHandler(int type)
{
    terminateSignal = true;
    STOP_TASK_MANAGER();
}

int32_t main(int32_t argc, const char* argv[])
{
#if defined(_MSC_VER)
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    //_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    GET_LOGGER().AddLogger([](const std::string& text)
    {
        printf("%s\n", text.c_str());
    });
    
    LOG("Service Started");
    LOG("press ctrl+c to terminate");

    // Set signal handlers for graceful termination
    signal(SIGABRT, SingalHandler);
    signal(SIGINT, SingalHandler);
    signal(SIGTERM, SingalHandler);

#ifndef _MSC_VER
    signal(SIGPIPE, SIG_IGN);
#endif

    std::string configFilePath;
    std::string protocol;
    std::string hostAddress;
    uint16_t hostPort = 0;
    std::string targetName;
    uint16_t targetPort = 0;

    const char* configArgs[] = {"--config", "-c"};
    const char* protocolArgs[] = {"--protocol", "-l"};
    const char* hostArgs[] = {"--host", "-h"};
    const char* portArgs[] = {"--port", "-p"};
    const char* targetArgs[] = { "--target", "-t" };
    const char* targetPortArgs[] = { "--target-port", "-q" };

    for (int32_t i = 1; i < argc; i++)
    {
        if (FIND_CMD(configArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            configFilePath = argv[++i];
        }
        else if (FIND_CMD(protocolArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            protocol = argv[++i];
        }
        else if (FIND_CMD(hostArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            hostAddress = argv[++i];
        }
        else if (FIND_CMD(portArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            hostPort = std::atoi(argv[++i]);
        }
        else if (FIND_CMD(targetArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            targetName = argv[++i];
        }
        else if (FIND_CMD(targetPortArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            targetPort = std::atoi(argv[++i]);
        }
        else
        {
            LOG("Unknown command line argument: %s", argv[i]);
        }
    }
    
    LOG("Configuration file: %s", configFilePath.c_str());
        
    // Create metricator service profile
    auto configuration = std::make_shared<ConfigurationXml>(configFilePath, "Service");
    if (protocol.empty())
    {
        configuration->GetValue("Protocol", protocol);
    }
    if (hostAddress.empty())
    {
        configuration->GetValue("HostAddress", hostAddress);
    }
    if (hostPort == 0)
    {
        configuration->GetValue("HostPort", hostPort);
    }
    if (targetName.empty())
    {
        configuration->GetValue("TargetName", targetName);
    }
    if (targetPort == 0)
    {
        configuration->GetValue("TargetPort", targetPort);
    }

    ConfigurationSingleton::InitializeConfiguration(configuration);

    auto bouncerDecoder = std::make_shared<BouncerDecoder>();
    auto profile = std::make_shared<BouncerProfile>();
    profile->Protocol.set(protocol);
    profile->Address.set(ANY_HOST);
    profile->Port.set(443);
    profile->Dispatcher.set(bouncerDecoder);
    profile->TargetName.set(targetName);
    profile->TargetPort.set(targetPort);

    // Create a REST service for settings
    BouncerRestService restService(profile);
    if (restService.Start())
    {
        profile->Address.set(hostAddress);
        profile->Port.set(hostPort);

        // Create a microservice
        Microservice service(profile);
        if (service.Start())
        {
            START_BLOCKING_TASK_LOOP();

            // Stop the microservice
            service.Stop();
        }
        else
        {
            LOG("Failed to start service");
        }
    }
    else
    {
        LOG("Failed to start the settings service");
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
