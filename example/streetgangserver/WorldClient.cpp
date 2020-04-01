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
    dispatcher->Protocol.set(protocol);
    dispatcher->Address.set(hostAddress);
    dispatcher->Port.set(port);

    auto connection = NetworkUtility::CreateConnection(dispatcher);
    Initialize(connection);
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
