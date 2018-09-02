#pragma warning(disable : 4512)

#include <signal.h>
#include <cstring>
#include <stdlib.h>
#include <thread>
#include <functional>
#include "Microreactor.h"
#include "UdpSocket.h"
#include "ConfigurationXml.h"
#include "Microservice.h"
#include "MetricatorDispatcher.h"
#include "DiscoveryService.h"
#include "GetDescriptionReactor.h"

using namespace sg::microreactor;
using namespace sg::service;
using namespace metricator;


volatile bool terminateSignal = false;
void SingalHandler(int type)
{
    terminateSignal = true;
    STOP_TASK_MANAGER();
}

std::shared_ptr<Reactor> CreateGetDescriptionReactor(std::shared_ptr<RestMessage> message, std::shared_ptr<Connection> connection)
{
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request->mUri == "/servicedescription.xml")
    {
        return nullptr;
    }

    auto reactor = std::make_shared<GetDescriptionReactor>(connection, request);
    return reactor;
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
    
    LOG("Metricator Started");
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
    std::string hostName;
    uint16_t hostPort = 0;

    const char* configArgs[] = {"--config", "-c"};
    const char* protocolArgs[] = {"--protocol", "-l"};
    const char* hostArgs[] = {"--host", "-h"};
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
    if (protocol.empty())
    {
        configuration->GetValue("Protocol", protocol);
    }
    if (hostName.empty())
    {
        configuration->GetValue("HostName", hostName);
    }
    if (hostPort == 0)
    {
        configuration->GetValue("HostPort", hostPort);
    }
    
    // Create DiscoveryService
    ServiceDescription description;
    description.ServerInfo.set("Metractor Service");
    description.Usn.set(Uuid::GenerateUuid().ToString());
    description.ServiceType.set("urn:streetgang:service:metricator:1");

#if 1
    auto discoveryService = std::make_shared<DiscoveryService>();
    discoveryService->Description.set(description);
    discoveryService->RegisterDescriptionReactorFactory("/servicedescription.xml", CreateGetDescriptionReactor);

    if (discoveryService->Start())
    {
        discoveryService->AdvertiseAlive();
    }
#else
    std::vector<std::shared_ptr<DiscoveryService>> discoveryServiceList;
    std::vector<NetworkUtility::NetworkInterfaceInfo> networkInterfaceInfoList;
    if (NetworkUtility::GetNetworkInterfaceInfo(networkInterfaceInfoList))
    {
        for (auto& networkInterface : networkInterfaceInfoList)
        {
            if (!networkInterface.mAddress.empty() && networkInterface.mAddress != "0.0.0.0")
            {
                auto discoveryService = std::make_shared<DiscoveryService>(networkInterface.mAddress, DEFAULT_MULTICAST_ADDRESS);
                discoveryService->Description.set(description);
                discoveryService->RegisterDescriptionReactorFactory("/servicedescription.xml", CreateGetDescriptionReactor);

                if (discoveryService->Start())
                {
                    discoveryService->AdvertiseAlive();
                    discoveryServiceList.emplace_back(discoveryService);
                }
                else
                {
                    LOG("Failed to start discovery server on interface %s", networkInterface.mAddress.c_str());
                }
            }
        }
    }
#endif

    auto messageDecoder = std::make_shared<MetricatorMessageDispatcher>();
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set(protocol);
    profile->Address.set(hostName);
    profile->Port.set(hostPort);
    profile->Dispatcher.set(messageDecoder);

    // Create metricatorService
    Microservice metricatorService(profile);
    if (metricatorService.Start())
    {
        START_BLOCKING_TASK_LOOP();
        
        // Stop DiscoveryService
        //for (auto& discoveryService : discoveryServiceList)
        //{
        //    disco/veryService->Stop();
        //}
        
        // Stop metricatorService
        metricatorService.Stop();
    }
    else
    {
        LOG("Failed to start metricator");
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
