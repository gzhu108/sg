#include "PBRequestByebye.h"
#include "StreetGangPBService.h"
#include "NetworkUtility.h"
#include "ConfigurationSingleton.h"
#include "StreetGangPBDispatcher.h"
#include "DiscoveryClient.h"
#include "Uuid.h"

using namespace microreactor;
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
        configuration->GetValue("TaskLatencyThreshold", mTaskLatencyThreshold);
    }
}

StreetGangPBService::~StreetGangPBService()
{
}

bool StreetGangPBService::Start()
{
    // Hook to task process
    //auto& taskProcessHook = GET_TASK_PROCESS_HOOK();
    //taskProcessHook.Preprocess.Connect(std::bind(&StreetGangPBService::OnTaskPreprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
    //taskProcessHook.Postprocess.Connect(std::bind(&StreetGangPBService::OnTaskPostprocess, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));

    return Service::Start();
}

bool StreetGangPBService::Stop()
{
    if (mListener != nullptr)
    {
        auto message = std::make_shared<PBRequestByebye>();
        message->TrackId.set(Uuid::GenerateUuid().ToString());

        std::stringstream stream;
        if (message->Encode(stream))
        {
            // Send byebye message
            SendAllConnections(stream);
        }

        // Disconnect to task process
        //auto& taskProcessHook = GET_TASK_PROCESS_HOOK();
        //taskProcessHook.Preprocess.Disconnect(reinterpret_cast<uintptr_t>(this));
        //taskProcessHook.Postprocess.Disconnect(reinterpret_cast<uintptr_t>(this));

        // Disable hot-config
        auto configuration = ConfigurationSingleton::GetConfiguration();
        if (configuration != nullptr)
        {
            configuration->ValueUpdated.Disconnect(mConfigurationConnectionId);
        }
    }

    return Service::Stop();
}

bool StreetGangPBService::Initialize()
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration == nullptr)
    {
        return false;
    }

    mConfigurationConnectionId = configuration->ValueUpdated.Connect(std::bind(&StreetGangPBService::Restart, this));

    uint32_t sendTimeout = 100;
    configuration->GetValue("SendTimeout", sendTimeout);

    auto dispatcher = std::make_shared<StreetGangPBDispatcher>();

    std::string protocol = "tcp";
    configuration->GetValue("Protocol", protocol);
    dispatcher->Protocol.set(protocol);

    std::string address = LOCAL_HOST;
    configuration->GetValue("ServiceAddress", address);
    dispatcher->Address.set(address);

    uint16_t port = 7390;
    configuration->GetValue("PBPort", port);
    dispatcher->Port.set(port);

    mListener = NetworkUtility::CreateListener(dispatcher);
    mListener->SendTimeout.set(std::chrono::milliseconds(sendTimeout));
    
    return true;
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
