#pragma warning(disable : 4512)

#include <signal.h>
#include <stdlib.h>
#include "Microreactor.h"
#include "SimpleWebService.h"

using namespace sg::microreactor;
using namespace simplewebserver;


volatile bool terminateSignal = false;
void SingalHandler(int type)
{
    terminateSignal = true;
    DESTROY_TASK_MANAGER();
}

int32_t main(int32_t argc, const char* argv[])
{
    GET_LOGGER().AddLogger([](const std::string& text)
    {
        printf("%s\n", text.c_str());
    });

    LOG("simplewebserver (%s)", sg::microreactor::StringUtility::GetHttpTimeString().c_str());
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

    ConfigurationSingleton::InitializeConfiguration(configuration);

    // Create the simple REST service
    auto simpleProfile = std::make_shared<Profile>();
    simpleProfile->Address.set(hostName);
    simpleProfile->Port.set(hostPort);

    auto simpleSocket = std::make_shared<TcpSocket>();
    auto simpleEndpoint = std::make_shared<TcpEndpoint>(simpleSocket, simpleProfile);
    simpleSocket = nullptr;

    SimpleWebService simpleWebService(simpleEndpoint);
    simpleEndpoint = nullptr;

    // Create the secure REST service
    auto secureProfile = std::make_shared<Profile>();
    secureProfile->Address.set(hostName);
    secureProfile->Port.set(securePort);

    auto secureSocket = std::make_shared<SecureTcpSocket>();
    secureSocket->ConfigureSslContext(SSLv23_server_method(), "selfsigned.key", "cert.pem", SecureTcpSocket::VerifyPeer);
    auto secureEndpoint = std::make_shared<TcpEndpoint>(secureSocket, secureProfile);
    secureSocket = nullptr;

    SimpleWebService secureWebService(secureEndpoint);
    secureEndpoint = nullptr;

    // Start the REST services
    if (simpleWebService.Start() && secureWebService.Start())
    {
        START_BLOCKING_TASK_LOOP();

        // Stop REST services
        simpleWebService.Stop();
        secureWebService.Stop();
    }
    else
    {
        LOG("Failed to start the simplewebserver");
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

    // Clean up OpenSSL
    EVP_cleanup();

    LOG("All done...");
    return 0;
}
