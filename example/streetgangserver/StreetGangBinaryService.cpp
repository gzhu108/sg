#include "StreetGangBinaryService.h"
#include "SecureTcpSocket.h"
#include "TcpEndpoint.h"
#include "ConfigurationSingleton.h"
#include "StreetGangBinaryDispatcher.h"
#include "DiscoveryRequester.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;
using namespace sg::service;

//CHECK_MEMORY_INIT

#define TASK_LATENCY_THRESHOLD ((double)0.01)


StreetGangBinaryService::StreetGangBinaryService()
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
    //taskProcessHook.Preprocess.Connect(std::bind(&StreetGangBinaryService::OnTaskPreprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
    //taskProcessHook.Postprocess.Connect(std::bind(&StreetGangBinaryService::OnTaskPostprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
}

StreetGangBinaryService::~StreetGangBinaryService()
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

bool StreetGangBinaryService::Initialize()
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration == nullptr)
    {
        return false;
    }

    mConfigurationConnectionId = configuration->ValueUpdated.Connect(std::bind(&StreetGangBinaryService::Restart, this));

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

    std::string address = "0.0.0.0";
    configuration->GetValue("ServiceAddress", address);
    profile->Address.set(address);

    uint16_t port = 8390;
    configuration->GetValue("ServicePort", port);
    profile->Port.set(port);

    profile->Dispatcher.set(std::make_shared<StreetGangBinaryDispatcher>());

    auto socket = std::make_shared<SecureTcpSocket>();
    socket->ConfigureSslContext(SSLv23_server_method(), "Server.key", "Server.cer", SecureTcpSocket::VerifyPeer);
    //auto socket = std::make_shared<TcpSocket>();
    mEndpoint = std::make_shared<TcpEndpoint>(socket, profile);
    LOG("SECURE TCP HOST: %s", mEndpoint->Name->c_str());

    mEndpoint->ListenTimeout.set(std::chrono::milliseconds(listenTimeout));
    mEndpoint->ReceiveTimeout.set(std::chrono::milliseconds(receiveTimeout));
    mEndpoint->SendTimeout.set(std::chrono::milliseconds(sendTimeout));

    return true;
}

void StreetGangBinaryService::SetResponseTime(std::shared_ptr<MessageBase> response)
{
    std::chrono::duration<double> responseTime = std::chrono::high_resolution_clock::now() - response->GetCreationTime();

    if (responseTime.count() > mTaskLatencyThreshold)
    {
        LOG("[Request=%d] [Latency=%f]", response->Id.cref(), responseTime.count());
    }
}

void StreetGangBinaryService::OnTaskPreprocess(const TaskPtr& task)
{
    //mTaskProcessTime[task.get()] = std::chrono::high_resolution_clock::now();
    uint64_t count = TaskManagerSingleton::GetTaskManager()->GetQueueDepth();
    LOG("Current queue depth = " FMT_UINT64 "", count);
}

void StreetGangBinaryService::OnTaskPostprocess(const TaskPtr& task)
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
