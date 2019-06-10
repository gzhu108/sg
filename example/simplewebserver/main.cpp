#pragma warning(disable : 4512)

#include <signal.h>
#include <stdlib.h>
#include "Microreactor.h"
#include "SimpleWebDispatcher.h"

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
    std::string hostAddress;
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
            hostAddress = argv[++i];
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
    if (hostAddress.empty())
    {
        configuration->GetValue("Address", hostAddress);
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
    auto simpleDispatcher = std::make_shared<SimpleWebDispatcher>();
    simpleDispatcher->Protocol.set("tcp");
    simpleDispatcher->Address.set(hostAddress);
    simpleDispatcher->Port.set(hostPort);

    //auto simpleSocket = std::make_shared<TcpSocket>();
    auto simpleEndpoint = std::make_shared<TcpEndpoint>(std::make_shared<TcpSocket>(), simpleDispatcher);
    Service simpleWebService(simpleEndpoint);
    simpleEndpoint = nullptr;

    // Create the secure REST service
    auto secureDispatcher = std::make_shared<SimpleWebDispatcher>();
    secureDispatcher->Protocol.set("tcp");
    secureDispatcher->Address.set(hostAddress);
    secureDispatcher->Port.set(securePort);

    auto secureSocket = std::make_shared<SecureTcpSocket>();
    secureSocket->ConfigureSslContext(SSLv23_server_method(), "selfsigned.key", "cert.pem", SecureTcpSocket::VerifyPeer);
    auto secureEndpoint = std::make_shared<TcpEndpoint>(secureSocket, secureDispatcher);
    secureSocket = nullptr;

    Service secureWebService(secureEndpoint);
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
    CANCEL_ALL_TASKS_AND_DESTROY_TASK_MANAGER();

    // Clean up OpenSSL
    EVP_cleanup();

    LOG("All done...");
    return 0;
}
