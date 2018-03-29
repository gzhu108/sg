#include "WorldService.h"
#include "NetworkUtility.h"
#include "ConfigurationSingleton.h"
#include "WorldDispatcher.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace worldserver;


WorldService::WorldService()
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration != nullptr)
    {
        // Get the thread pool size from the configuration file
        uint64_t threadPoolSize = 0;
        configuration->GetValue("ThreadPoolSize", threadPoolSize);
        TaskManagerSingleton::SetThreadPoolSize(threadPoolSize);
    }
}

WorldService::~WorldService()
{
    Stop();

    // Disable hot-config
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration != nullptr)
    {
        configuration->ValueUpdated.Disconnect(mConfigurationConnectionId);
    }
}

bool WorldService::Start()
{
    if (CreateBinaryListener())
    {
        // Start the endpoint
        if (mEndpoint != nullptr)
        {
            if (!mEndpoint->Start())
            {
                return false;
            }

            // Connection to the ConnectonMade signal
            mEndpoint->ConnectionMade.Connect(std::bind(&WorldService::OnConnectionMade, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
            return true;
        }
    }

    return false;
}

bool WorldService::Stop()
{
    // Stop all listeners
    if (mEndpoint != nullptr)
    {
        if (!mEndpoint->Stop())
        {
            return false;
        }

        // Disconnect the ConnectonMade signal
        mEndpoint->ConnectionMade.Disconnect(reinterpret_cast<uintptr_t>(this));
        mEndpoint = nullptr;
        return true;
    }

    return false;
}

bool WorldService::CreateBinaryListener()
{
    auto configuration = ConfigurationSingleton::GetConfiguration();
    if (configuration == nullptr)
    {
        return false;
    }

    mConfigurationConnectionId = configuration->ValueUpdated.Connect(std::bind(&WorldService::Restart, this));

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

    profile->Dispatcher.set(std::make_shared<WorldDispatcher>());

    mEndpoint = NetworkUtility::CreateEndpoint(profile);
    mEndpoint->ListenTimeout.set(std::chrono::milliseconds(listenTimeout));
    mEndpoint->ReceiveTimeout.set(std::chrono::milliseconds(receiveTimeout));
    mEndpoint->SendTimeout.set(std::chrono::milliseconds(sendTimeout));

    return true;
}

void WorldService::OnConnectionMade(const std::shared_ptr<const Connection>& connection)
{
    std::string hostName = connection->Name.cref();
    std::string connectionName = std::string("[") + connection->GetPeerName() + "]:" + std::to_string(connection->GetPeerPort());

    connection->Closed.Connect([=]()
    {
        LOG("Disconnected %s -> %s", hostName.c_str(), connectionName.c_str());
    });

    LOG("Connection accepted %s -> %s", hostName.c_str(), connectionName.c_str());
}
