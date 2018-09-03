#include "WorldService.h"
#include "NetworkUtility.h"
#include "ConfigurationSingleton.h"
#include "WorldDispatcher.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace worldserver;


WorldService::WorldService()
{
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

bool WorldService::Initialize()
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
