#pragma warning(disable : 4512)

#include "google/protobuf/stubs/common.h"
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <functional>
#include <vector>
#include "Microreactor.h"
#include "NetworkUtility.h"
#include "StreetGangService.h"
#include "ConfigurationXml.h"
#include "MetricatorLogger.h"
//#include "UdpSocket.h"
//#include "DiscoveryService.h"

using namespace sg::microreactor;
using namespace metricatorapi;
using namespace streetgangapi;
using namespace streetgangserver;


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
#if defined(_MSC_VER)
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    //_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    ConsoleLogger logger;
    GET_LOGGER().AddLogger(std::bind(&ConsoleLogger::Log, &logger, std::placeholders::_1));

    LOG("Street Gang Server Started");
    LOG("press ctrl+c to terminate");

    // Set signal handlers for graceful termination
    signal(SIGABRT, SingalHandler);
    signal(SIGINT, SingalHandler);
    signal(SIGTERM, SingalHandler);

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

    // Create the metricator client for logging
    std::string metricatorProtocol = "udp";
    std::string metricatorHost = "127.0.0.1";
    uint16_t metricatorPort = 0;

    configuration->GetValue("MetricatorProtocol", metricatorProtocol);
    configuration->GetValue("MetricatorHost", metricatorHost);
    configuration->GetValue("MetricatorPort", metricatorPort);

    MetricatorLogger metricatorLogger(metricatorProtocol, metricatorHost, metricatorPort);
    GET_LOGGER().AddLogger(std::bind(&MetricatorLogger::Log, &metricatorLogger, std::placeholders::_1));

    // Determine the service IP address
    std::string address = "127.0.0.1";
    std::vector<NetworkUtility::NetworkInterfaceInfo> networkInterfaceInfos;
    NetworkUtility::GetNetworkInterfaceInfo(networkInterfaceInfos);
    for (auto& networkInterface : networkInterfaceInfos)
    {
        if (!networkInterface.mAddress.empty() && networkInterface.mAddress != "0.0.0.0")
        {
            address = networkInterface.mAddress;
            break;
        }
    }
    
#if 0
    // Create the socket for DiscoveryService
    auto socket = std::make_shared<UdpSocket>();
    socket->Bind(address, (uint16_t)1900);
    
    // Create DiscoveryService
    DiscoveryService discoveryService(socket);
    discoveryService.Start();

    // Create the discovery client
    std::string metricatorServiceType = "urn:streetgang:service:metricator:1";
    profile->GetConfiguration()->GetValue("MetricatorServiceType", metricatorServiceType);

    std::shared_ptr<UdpConnection> discoveryConnection = std::make_shared<UdpConnection>(discoverySocket);
    discoveryConnection->Source.set(multicastAddress);
    discoveryConnection->Port.set(multicastPort);

    mDiscoveryClient = std::make_shared<Client>();
    mDiscoveryClient->Initialize(profile, discoveryConnection, std::chrono::milliseconds(1000));
    DiscoveryApi discoveryApi(*mDiscoveryClient);
    discoveryApi.MulticastSearch(metricatorServiceType, multicastAddress, multicastPort);

    // Stop DiscoveryService
    discoveryService.Stop();
#endif
    
    //auto profile = std::make_shared<Profile>();
    //profile->Protocol.set(std::string("tcp"));
    //profile->Address.set(std::string("0.0.0.0"));
    //profile->Port.set(8390);
    //Microservice streetGangService(profile);

    // Create StreetGangService
    StreetGangService streetGangService(configuration);
    if (streetGangService.Start())
    {
        START_BLOCKING_TASK_LOOP();

        // Stop StreetGangService
        streetGangService.Stop();
    }
    else
    {
        LOG("Failed to start the streetgangserver");
    }

    // Optional: Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    LOG("All done...");
    return 0;
}
