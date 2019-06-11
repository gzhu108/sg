#include "RestService.h"
#include "NetworkUtility.h"

using namespace sg::microreactor;


RestService::RestService(const std::string& hostName, uint16_t port)
{
    auto dispatcher = std::make_shared<RestDispatcher>();
    dispatcher->Protocol.set("tcp");
    dispatcher->Address.set(hostName);
    dispatcher->Port.set(port);
    mEndpoint = NetworkUtility::CreateEndpoint(dispatcher);
}

RestService::RestService(std::shared_ptr<Profile> profile)
{
    auto dispatcher = std::make_shared<RestDispatcher>();

    if (profile == nullptr)
    {
        dispatcher->Protocol.set("tcp");
        dispatcher->Address.set(ANY_HOST);
        dispatcher->Port.set(80);
    }
    else
    {
        dispatcher->Protocol.set(profile->Protocol.cref());
        dispatcher->Address.set(profile->Address.cref());
        dispatcher->Port.set(profile->Port.cref());
    }

    mEndpoint = NetworkUtility::CreateEndpoint(dispatcher);
}

RestService::~RestService()
{
}

void RestService::OnConnectionMade(const std::shared_ptr<Connection>& connection)
{
    std::string hostName = connection->Name.cref();
    std::string connectionName = std::string("[") + connection->GetPeerAddress() + "]:" + std::to_string(connection->GetPeerPort());

    connection->Closed.Connect([=]()
    {
        LOG("Disconnected %s -> %s", hostName.c_str(), connectionName.c_str());
    });

    LOG("Connection accepted %s -> %s", hostName.c_str(), connectionName.c_str());
}
