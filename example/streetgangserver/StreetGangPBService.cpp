#include "StreetGangPBService.h"
#include "NetworkUtility.h"
#include "ConfigurationSingleton.h"
#include "StreetGangPBDispatcher.h"
#include "DiscoveryRequester.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;
using namespace sg::service;

//CHECK_MEMORY_INIT

#define TASK_LATENCY_THRESHOLD ((double)0.01)


StreetGangPBService::StreetGangPBService()
    : mTaskLatencyThreshold(TASK_LATENCY_THRESHOLD)
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration != nullptr)
    {
        // Get the thread pool size from the configuration file
        uint64_t threadPoolSize = 0;
        configuration->GetValue("ThreadPoolSize", threadPoolSize);
        TaskManagerSingleton::SetThreadPoolSize(threadPoolSize);

        configuration->GetValue("TaskLatencyThreshold", mTaskLatencyThreshold);
    }

    // Hook to task process
    //auto& taskProcessHook = GET_TASK_PROCESS_HOOK();
    //taskProcessHook.Preprocess.Connect(std::bind(&StreetGangPBService::OnTaskPreprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
    //taskProcessHook.Postprocess.Connect(std::bind(&StreetGangPBService::OnTaskPostprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
}

StreetGangPBService::~StreetGangPBService()
{
    Stop();

    // Disable hot-config
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration != nullptr)
    {
        configuration->ValueUpdated.Disconnect(mConfigurationConnectionId);
    }

    // Disconnect to task process
    auto& taskProcessHook = GET_TASK_PROCESS_HOOK();
    taskProcessHook.Preprocess.Disconnect(reinterpret_cast<uintptr_t>(this));
    taskProcessHook.Postprocess.Disconnect(reinterpret_cast<uintptr_t>(this));
}

bool StreetGangPBService::Start()
{
    if (CreatePBListener())
    {
        if (mEndpoint != nullptr)
        {
            if (!mEndpoint->Start())
            {
                return false;
            }

            // Connection to the ConnectonMade signal
            mEndpoint->ConnectionMade.Connect(std::bind(&StreetGangPBService::OnConnectionMade, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
            return true;
        }
    }

    return false;
}

bool StreetGangPBService::Stop()
{
    // Stop endpoint
    if (mEndpoint != nullptr)
    {
        if (!mEndpoint->Stop())
        {
            return false;
        }

        // Disconnect the ConnectonMade signal
        mEndpoint->ConnectionMade.Disconnect(reinterpret_cast<uintptr_t>(this));
        return true;
    }

    return false;
}

bool StreetGangPBService::CreatePBListener()
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration == nullptr)
    {
        return false;
    }

    mConfigurationConnectionId = configuration->ValueUpdated.Connect(std::bind(&StreetGangPBService::Restart, this));

    uint32_t listenTimeout = 30;
    uint32_t receiveTimeout = 30;
    uint32_t sendTimeout = 100;

    configuration->GetValue("ListenTimeout", listenTimeout);
    configuration->GetValue("ReceiveTimeout", receiveTimeout);
    configuration->GetValue("SendTimeout", sendTimeout);

    auto profile = std::make_shared<Profile>();

    std::string protocol = "tcp";
    configuration->GetValue("Protocol", protocol);
    profile->Protocol.set(protocol);

    std::string address = "127.0.0.1";
    configuration->GetValue("ServiceAddress", address);
    profile->Address.set(address);

    uint16_t port = 7390;
    configuration->GetValue("PBPort", port);
    profile->Port.set(port);

    profile->Dispatcher.set(std::make_shared<StreetGangPBDispatcher>());

    mEndpoint = NetworkUtility::CreateEndpoint(profile);
    mEndpoint->ListenTimeout.set(std::chrono::milliseconds(listenTimeout));
    mEndpoint->ReceiveTimeout.set(std::chrono::milliseconds(receiveTimeout));
    mEndpoint->SendTimeout.set(std::chrono::milliseconds(sendTimeout));
    
    return true;
}

void StreetGangPBService::OnConnectionMade(const std::shared_ptr<const Connection>& connection)
{
    std::string hostName = connection->Name.cref();
    std::string connectionName = std::string("[") + connection->GetPeerName() + "]:" + std::to_string(connection->GetPeerPort());

    connection->Closed.Connect([=]()
    {
        LOG("Disconnected %s -> %s", hostName.c_str(), connectionName.c_str());
    });

    LOG("Connection accepted %s -> %s", hostName.c_str(), connectionName.c_str());
}

void StreetGangPBService::SetResponseTime(std::shared_ptr<MessageBase> response)
{
    std::chrono::duration<double> responseTime = std::chrono::high_resolution_clock::now() - response->GetCreationTime();

    if (responseTime.count() > mTaskLatencyThreshold)
    {
        LOG("[Request=%d] [Latency=%f]", response->Id.cref(), responseTime.count());
    }
}

void StreetGangPBService::OnTaskPreprocess(const TaskPtr& task)
{
    //mTaskProcessTime[task.get()] = std::chrono::high_resolution_clock::now();
    uint64_t count = TaskManagerSingleton::GetTaskManager()->GetQueueDepth();
    LOG("Current queue depth = " FMT_UINT64 "", count);
}

void StreetGangPBService::OnTaskPostprocess(const TaskPtr& task)
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
