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

    uint32_t receiveTimeout = 30;
    uint32_t sendTimeout = 100;

    configuration->GetValue("ReceiveTimeout", receiveTimeout);
    configuration->GetValue("SendTimeout", sendTimeout);

    auto dispatcher = std::make_shared<WorldDispatcher>();

    std::string protocol = "tcp";
    configuration->GetValue("Protocol", protocol);
    dispatcher->Protocol.set(protocol);

    std::string address = ANY_HOST;
    configuration->GetValue("ServiceAddress", address);
    dispatcher->Address.set(address);

    uint16_t port = 8390;
    configuration->GetValue("ServicePort", port);
    dispatcher->Port.set(port);

    mListener = NetworkUtility::CreateListener(dispatcher);
    mListener->ReceiveTimeout.set(std::chrono::milliseconds(receiveTimeout));
    mListener->SendTimeout.set(std::chrono::milliseconds(sendTimeout));

    return true;
}
