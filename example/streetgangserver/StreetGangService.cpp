#include "StreetGangService.h"
#include "NetworkUtility.h"
#include "ConfigurationXml.h"
#include "StreetGangMessageDecoder.h"
#include "StreetGangPBDecoder.h"
#include "DiscoveryApi.h"
#include "RestMessageDecoder.h"
#include "RestGetVersion.h"
#include "RestCreateWorld.h"
#include "RestGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;
using namespace sg::service;

//CHECK_MEMORY_INIT

#define TASK_LATENCY_THRESHOLD ((double)0.01)


StreetGangService::StreetGangService(std::shared_ptr<Configuration> configuration)
    : mConfiguration(configuration)
    , mTaskLatencyThreshold(TASK_LATENCY_THRESHOLD)
{
    // Get the thread pool size from the configuration file
    uint64_t threadPoolSize = 0;
    mConfiguration->GetValue("ThreadPoolSize", threadPoolSize);
    TaskManagerSingleton::SetThreadPoolSize(threadPoolSize);

    mConfiguration->GetValue("TaskLatencyThreshold", mTaskLatencyThreshold);

    // Hook to task process
    //auto& taskProcessHook = GET_TASK_PROCESS_HOOK();
    //taskProcessHook.Preprocess.Connect(std::bind(&StreetGangService::OnTaskPreprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
    //taskProcessHook.Postprocess.Connect(std::bind(&StreetGangService::OnTaskPostprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
}

StreetGangService::~StreetGangService()
{
    Stop();

    // Disable hot-config
    if (mConfiguration != nullptr)
    {
        mConfiguration->ValueUpdated.Disconnect(mConfigurationConnectionId);
    }

    // Disconnect to task process
    auto& taskProcessHook = GET_TASK_PROCESS_HOOK();
    taskProcessHook.Preprocess.Disconnect(reinterpret_cast<uintptr_t>(this));
    taskProcessHook.Postprocess.Disconnect(reinterpret_cast<uintptr_t>(this));

    mListenerCollection.clear();
}

bool StreetGangService::Start()
{
    if (CreateListeners())
    {
        bool result = true;

        // Start all listeners
        for (auto& listener : mListenerCollection)
        {
            if (!listener->Start())
            {
                result = false;
            }

            // Connection to the ConnectonMade signal
            auto host = listener->GetHost();
            if (host != nullptr)
            {
                host->ConnectionMade.Connect(std::bind(&StreetGangService::OnConnectionMade, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
            }
        }

        return result;
    }

    return false;
}

bool StreetGangService::Stop()
{
    bool result = true;

    // Stop all listeners
    for (auto& listener : mListenerCollection)
    {
        if (!listener->Stop())
        {
            result = false;
        }

        // Disconnect the ConnectonMade signal
        auto host = listener->GetHost();
        if (host != nullptr)
        {
            host->ConnectionMade.Disconnect(reinterpret_cast<uintptr_t>(this));
        }
    }

    return result;
}

bool StreetGangService::CreateListeners()
{
    if (mConfiguration == nullptr)
    {
        return false;
    }

    CreateBinaryListener();
    CreatePBListener();
    CreateRestListener();
    
    mConfigurationConnectionId = mConfiguration->ValueUpdated.Connect(std::bind(&StreetGangService::Restart, this));
    return !mListenerCollection.empty();
}

bool StreetGangService::CreateBinaryListener()
{
    if (mConfiguration == nullptr)
    {
        return false;
    }

    uint32_t listenTimeout = 30;
    uint32_t receiveTimeout = 30;
    uint32_t sendTimeout = 100;

    mConfiguration->GetValue("ListenTimeout", listenTimeout);
    mConfiguration->GetValue("ReceiveTimeout", receiveTimeout);
    mConfiguration->GetValue("SendTimeout", sendTimeout);

    auto profile = std::make_shared<Profile>();
    profile->Configuration.set(mConfiguration);

    std::string protocol = "tcp";
    mConfiguration->GetValue("Protocol", protocol);
    profile->Protocol.set(protocol);

    std::string address = "0.0.0.0";
    mConfiguration->GetValue("ServiceAddress", address);
    profile->Address.set(address);

    uint16_t port = 8390;
    mConfiguration->GetValue("ServicePort", port);
    profile->Port.set(port);

    profile->Dispatcher.set(std::make_shared<StreetGangMessageDecoder>());

    std::shared_ptr<Host> host = NetworkUtility::CreateHost(profile);
    host->ListenTimeout.set(std::chrono::milliseconds(listenTimeout));
    host->ReceiveTimeout.set(std::chrono::milliseconds(receiveTimeout));
    host->SendTimeout.set(std::chrono::milliseconds(sendTimeout));

    auto listener = std::make_shared<Listener>();
    if (listener->Initialize(host))
    {
        mListenerCollection.emplace_back(listener);
        return true;
    }

    return false;
}

bool StreetGangService::CreatePBListener()
{
    if (mConfiguration == nullptr)
    {
        return false;
    }

    uint32_t listenTimeout = 30;
    uint32_t receiveTimeout = 30;
    uint32_t sendTimeout = 100;

    mConfiguration->GetValue("ListenTimeout", listenTimeout);
    mConfiguration->GetValue("ReceiveTimeout", receiveTimeout);
    mConfiguration->GetValue("SendTimeout", sendTimeout);

    auto profile = std::make_shared<Profile>();
    profile->Configuration.set(mConfiguration);

    std::string protocol = "tcp";
    mConfiguration->GetValue("Protocol", protocol);
    profile->Protocol.set(protocol);

    std::string address = "127.0.0.1";
    mConfiguration->GetValue("ServiceAddress", address);
    profile->Address.set(address);

    uint16_t port = 7390;
    mConfiguration->GetValue("PBPort", port);
    profile->Port.set(port);

    profile->Dispatcher.set(std::make_shared<StreetGangPBDecoder>());

    std::shared_ptr<Host> host = NetworkUtility::CreateHost(profile);
    host->ListenTimeout.set(std::chrono::milliseconds(listenTimeout));
    host->ReceiveTimeout.set(std::chrono::milliseconds(receiveTimeout));
    host->SendTimeout.set(std::chrono::milliseconds(sendTimeout));
    
    auto listener = std::make_shared<Listener>();
    if (listener->Initialize(host))
    {
        mListenerCollection.emplace_back(listener);
        return true;
    }

    return false;
}

bool StreetGangService::CreateRestListener()
{
    if (mConfiguration == nullptr)
    {
        return false;
    }

    uint32_t listenTimeout = 30;
    uint32_t receiveTimeout = 30;
    uint32_t sendTimeout = 100;

    mConfiguration->GetValue("ListenTimeout", listenTimeout);
    mConfiguration->GetValue("ReceiveTimeout", receiveTimeout);
    mConfiguration->GetValue("SendTimeout", sendTimeout);

    auto restMessageDecoder = std::make_shared<RestMessageDecoder>();
    restMessageDecoder->RegisterRestApi(std::make_shared<RestGetVersion>());
    restMessageDecoder->RegisterRestApi(std::make_shared<RestCreateWorld>());
    restMessageDecoder->RegisterRestApi(std::make_shared<RestGetScene>());

    std::string serviceAddress = "0.0.0.0";
    uint16_t restPort = 9390;
    mConfiguration->GetValue("ServiceAddress", serviceAddress);
    mConfiguration->GetValue("RestPort", restPort);

    auto profile = std::make_shared<Profile>();
    profile->Configuration.set(mConfiguration);
    profile->Protocol.set("tcp");
    profile->Address.set(serviceAddress);
    profile->Port.set(restPort);
    profile->Dispatcher.set(restMessageDecoder);

    std::shared_ptr<Host> host = NetworkUtility::CreateHost(profile);
    host->ListenTimeout.set(std::chrono::milliseconds(listenTimeout));
    host->ReceiveTimeout.set(std::chrono::milliseconds(receiveTimeout));
    host->SendTimeout.set(std::chrono::milliseconds(sendTimeout));
    
    auto listener = std::make_shared<Listener>();
    if (listener->Initialize(host))
    {
        mListenerCollection.emplace_back(listener);
        return true;
    }

    return false;
}

void StreetGangService::OnConnectionMade(const std::shared_ptr<const Connection>& connection)
{
    std::string hostName = connection->Name.cref();
    std::string connectionName = std::string("[") + connection->GetPeerName() + "]:" + std::to_string(connection->GetPeerPort());

    connection->Closed.Connect([=]()
    {
        LOG("Disconnected %s -> %s", hostName.c_str(), connectionName.c_str());
    });

    LOG("Connection accepted %s -> %s", hostName.c_str(), connectionName.c_str());
}

void StreetGangService::SetResponseTime(std::shared_ptr<MessageBase> response)
{
    std::chrono::duration<double> responseTime = std::chrono::high_resolution_clock::now() - response->GetCreationTime();

    if (responseTime.count() > mTaskLatencyThreshold)
    {
        LOG("[Request=%d] [Latency=%f]", response->Id.cref(), responseTime.count());
    }
}

void StreetGangService::OnTaskPreprocess(const TaskPtr& task)
{
    //mTaskProcessTime[task.get()] = std::chrono::high_resolution_clock::now();
    uint64_t count = TaskManagerSingleton::GetTaskManager()->GetQueueDepth();
    LOG("Current queue depth = " FMT_UINT64 "", count);
}

void StreetGangService::OnTaskPostprocess(const TaskPtr& task)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> processTimeEnd = std::chrono::high_resolution_clock::now();

    auto taskItr = mTaskProcessTime.find(task.get());
    if (taskItr != mTaskProcessTime.end())
    {
        std::chrono::duration<double> processTimeUsed = processTimeEnd - taskItr->second;
        mTaskProcessTime.erase(taskItr);

        if (processTimeUsed.count() > mTaskLatencyThreshold)
        {
            LOG("[Task=%s] [Latency=%f]", task->Name->c_str(), processTimeUsed.count());
        }
    }
    else
    {
        LOG("Task (%p) not found", task.get());
    }
}
