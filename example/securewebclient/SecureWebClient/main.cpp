#pragma warning(disable : 4512)

#include <signal.h>
#include <stdlib.h>
#include "Microreactor.h"

using namespace microreactor;


volatile bool terminateSignal = false;
void SingalHandler(int type)
{
    terminateSignal = true;
    STOP_TASK_MANAGER();
}

int32_t main(int32_t argc, const char* argv[])
{
    GET_LOGGER().AddLogger([](const std::string& text)
    {
        printf("%s\n", text.c_str());
    });

    LOG("simplewebserver (%s)", microreactor::StringUtility::GetHttpTimeString().c_str());
    LOG("press ctrl+c to terminate");

    // Set signal handlers for graceful termination
    signal(SIGABRT, SingalHandler);
    signal(SIGINT, SingalHandler);
    signal(SIGTERM, SingalHandler);

#ifndef _MSC_VER
    signal(SIGPIPE, SIG_IGN);
#endif

    std::string configFilePath;
    std::string hostName;
    uint16_t hostPort = 0;
    uint16_t securePort = 0;

    const char* configArgs[] = {"--config", "-c"};
    const char* hostArgs[] = {"--host", "-h"};
    const char* portArgs[] = {"--port", "-p"};
    const char* securePortArgs[] = {"--secure-port", "-s"};

    for (int32_t i = 1; i < argc; ++i)
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
        else if (FIND_CMD(securePortArgs, argv[i]))
        {
            LOG("Command line argument: %s %s", argv[i], argv[i + 1]);
            securePort = std::atoi(argv[++i]);
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

    // Create the secure REST client
    auto secureProfile = std::make_shared<Profile>();
    secureProfile->Address.set(hostName);
    secureProfile->Port.set(securePort);
    secureProfile->Configuration.set(configuration);

    auto secureSocket = std::make_shared<SecureTcpSocket>();
    secureSocket->ConfigureClientContext();
    secureSocket->Connect(hostName, securePort, std::chrono::milliseconds(30));

    auto secureConnection = std::make_shared<TcpConnection>(secureSocket, secureProfile);
    secureSocket = nullptr;

    auto secureClient = std::make_shared<Client>();
    secureClient->Initialize(secureConnection, std::chrono::milliseconds(30));

    START_BLOCKING_TASK_LOOP();

    // Clean up OpenSSL
    EVP_cleanup();

    LOG("All done...");
    return 0;
}
