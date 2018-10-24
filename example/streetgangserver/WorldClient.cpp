#include "WorldClient.h"
#include "NetworkUtility.h"
#include "ConfigurationSingleton.h"
#include "WorldClientDispatcher.h"
#include "worldapi/ResponseError.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


std::shared_ptr<WorldClient> WorldClient::mInstance;

WorldClient::WorldClient(const std::string& protocol, const std::string& hostAddress, uint16_t port)
    : mWorldCache(std::make_shared<WorldCache>())
{
    auto dispatcher = std::make_shared<WorldClientDispatcher>();

    // Create client profile
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set(protocol);
    profile->Address.set(hostAddress);
    profile->Port.set(port);
    profile->Dispatcher.set(dispatcher);

    auto connection = NetworkUtility::CreateConnection(profile);
    Initialize(connection, std::chrono::milliseconds(30));
}

WorldClient::~WorldClient()
{
}

WorldClient& WorldClient::GetInstance()
{
    if (mInstance == nullptr)
    {
        std::string protocol;
        std::string worldServerHost;
        uint16_t worldServerPort = 0;

        auto configuration = ConfigurationSingleton::GetConfiguration();
        configuration->GetValue("Protocol", protocol);
        configuration->GetValue("WorldServerHost", worldServerHost);
        configuration->GetValue("WorldServerPort", worldServerPort);

        mInstance.reset(new WorldClient(protocol, worldServerHost, worldServerPort));
    }

    return *mInstance;
}

void WorldClient::ResetWorldClient()
{
    mInstance = nullptr;
}
