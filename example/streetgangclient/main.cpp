#pragma warning(disable : 4512)

#include "google/protobuf/stubs/common.h"
#include <chrono>
#include <thread>
#include <signal.h>
#include <stdlib.h>
#include "Microreactor.h"
#include "StreetGangApi.h"
#include "StreetGangClient.h"
#include "ConfigurationXml.h"
#include "ResponseGetSceneReactor.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


volatile bool terminateSignal = false;
void SingalHandler(int type)
{
    terminateSignal = true;
    STOP_TASK_MANAGER();
}

int32_t main(int32_t argc, const char* argv[])
{
#if defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    GET_LOGGER().AddLogger([](const std::string& text)
    {
        printf("%s\n", text.c_str());
    });

    LOG("Street Gang Client Started");
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
    std::string serverAddress;
    uint16_t serverPort = 0;

    const char* configArgs[] = {"--config", "-c"};
    const char* protocolArgs[] = {"--protocol", "-l"};
    const char* serverArgs[] = {"--server", "-s"};
    const char* portArgs[] = {"--port", "-p"};

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
        else if (FIND_CMD(serverArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            serverAddress = argv[++i];
        }
        else if (FIND_CMD(portArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            serverPort = std::atoi(argv[++i]);
        }
        else
        {
            LOG("Unknown command line argument: %s", argv[i]);
        }
    }
    
    LOG("Configuration file: %s", configFilePath.c_str());
        
    // Create metricator service profile
    auto configuration = std::make_shared<ConfigurationXml>(configFilePath, "Client");
    if (protocol.empty())
    {
        configuration->GetValue("Protocol", protocol);
    }
    if (serverAddress.empty())
    {
        configuration->GetValue("ServerAddress", serverAddress);
    }
    if (serverPort == 0)
    {
        configuration->GetValue("ServerPort", serverPort);
    }
    
    LOG("server address: %s, server port: %d", serverAddress.c_str(), serverPort);    
    auto streetGangClient = std::make_shared<StreetGangClient>(protocol, serverAddress, serverPort);

    auto responseGetSceneReactor = std::make_shared<ResponseGetSceneReactor>(*streetGangClient->GetConnection(), nullptr, streetGangClient->GetStreetGangRequester());
    responseGetSceneReactor->SetMessageEncoder(streetGangClient->GetStreetGangRequestEncoder());
    SUBMIT(std::bind(&ResponseGetSceneReactor::SendNextRequest, responseGetSceneReactor), responseGetSceneReactor, 0, "ResponseGetSceneReactor::SendNextRequest");
    responseGetSceneReactor = nullptr;

    START_BLOCKING_TASK_LOOP();

    // Optional: Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    LOG("All done...");

    return 0;
}
