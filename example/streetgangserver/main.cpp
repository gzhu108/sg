#pragma warning(disable : 4512)

#include "google/protobuf/stubs/common.h"
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <functional>
#include <vector>
#include "Microreactor.h"
#include "StreetGangBinaryService.h"
#include "StreetGangPBService.h"
#include "StreetGangRestDispatcher.h"
#include "WorldClient.h"
#include "ConfigurationXml.h"
#include "MetricatorLogger.h"
#include "DiscoveryClient.h"

using namespace microreactor;
using namespace sg::service;
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

    class MSearchTask : public BaseTask
    {
    public:
        explicit MSearchTask(DiscoveryClient& discoveryClient, const std::chrono::milliseconds& searchTime)
            : mDiscoveryClient(discoveryClient)
            , mSearchTime(searchTime)
        {
            SetExecuteTime(std::chrono::high_resolution_clock::now() + mSearchTime);
            Completed.Connect([&]() {
                mStatus = TaskStatus::Scheduled;
            });
        }

        virtual ~MSearchTask()
        {
        }

        virtual void Run() override
        {
            mDiscoveryClient.MulticastMSearch(DEFAULT_MULTICAST_ADDRESS, DEFAULT_MULTICAST_PORT, "2");
            SetExecuteTime(std::chrono::high_resolution_clock::now() + mSearchTime);
        }

    private:
        DiscoveryClient& mDiscoveryClient;
        std::chrono::milliseconds mSearchTime;
    };
}

static std::shared_ptr<Service> CreateRestService()
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration == nullptr)
    {
        return nullptr;
    }

    uint32_t sendTimeout = 100;

    configuration->GetValue("SendTimeout", sendTimeout);

    std::string serviceAddress = ANY_HOST;
    uint16_t restPort = 9390;
    configuration->GetValue("ServiceAddress", serviceAddress);
    configuration->GetValue("RestPort", restPort);

    auto dispatcher = std::make_shared<StreetGangRestDispatcher>();
    dispatcher->Protocol.set("tcp");
    dispatcher->Address.set(serviceAddress);
    dispatcher->Port.set(restPort);

    std::shared_ptr<Listener> listener = NetworkUtility::CreateListener(dispatcher);
    listener->SendTimeout.set(std::chrono::milliseconds(sendTimeout));

    return std::make_shared<Service>(listener);
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
    
    auto dispatcher = std::make_shared<DiscoveryDispatcher>();
    DiscoveryClient discoveryClient(dispatcher);

    const std::string metricatorServiceType = "urn:streetgang:service:metricator:1";
    discoveryClient.ServiceType.set(metricatorServiceType);

    auto timedTask = std::make_shared<MSearchTask>(discoveryClient, std::chrono::milliseconds(2000));
    timedTask->Name.set("MSearch");
    SUBMIT_TASK(std::static_pointer_cast<Task>(timedTask));

    discoveryClient.ServiceFound.Connect([](const ServiceDescription& description)
    {
        LOG("M_SEARCH OK received:");
        LOG("%s", description.Location->c_str());
    });

    discoveryClient.Byebye.Connect([](const Uuid& usn)
    {
        LOG("BYEBYE received:");
        LOG("%s", usn.ToString().c_str());
    });

    // Create the metricator client for logging
    std::string metricatorProtocol = "udp";
    std::string metricatorHost = LOCAL_HOST;
    uint16_t metricatorPort = 0;

    configuration->GetValue("MetricatorProtocol", metricatorProtocol);
    configuration->GetValue("MetricatorHost", metricatorHost);
    configuration->GetValue("MetricatorPort", metricatorPort);

    //MetricatorLogger metricatorLogger(metricatorProtocol, metricatorHost, metricatorPort);
    //GET_LOGGER().AddLogger(std::bind(&MetricatorLogger::Log, &metricatorLogger, std::placeholders::_1));

    std::vector<std::shared_ptr<Service>> services;
    services.emplace_back(std::make_shared<StreetGangBinaryService>());
    services.emplace_back(std::make_shared<StreetGangPBService>());
    services.emplace_back(CreateRestService());

    if (!Application::Context().Run(services))
    {
        LOG("Failed to start the streetgangserver");
    }

    // Reset the world client
    WorldClient::ResetWorldClient();

    // Optional: Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    LOG("All done...");
    return 0;
}
