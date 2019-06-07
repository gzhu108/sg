#include "RestService.h"

using namespace sg::microreactor;


RestService::RestService()
{
}

RestService::RestService(const std::string& hostName, uint16_t port)
    : mRestDispatcher(std::make_shared<RestDispatcher>())
{
    std::shared_ptr<Profile> profile = std::make_shared<Profile>();
    profile->Protocol.set("tcp");
    profile->Address.set(hostName);
    profile->Port.set(port);
    profile->Dispatcher.set(mRestDispatcher);
    mEndpoint = NetworkUtility::CreateEndpoint(profile);
}

RestService::RestService(std::shared_ptr<Profile> profile)
    : Service()
{
    if (profile != nullptr)
    {
        if (profile->Dispatcher.cref() == nullptr)
        {
            mRestDispatcher = std::make_shared<RestDispatcher>();
            profile->Dispatcher.set(mRestDispatcher);
        }
        else
        {
            mRestDispatcher = std::static_pointer_cast<RestDispatcher>(profile->Dispatcher.cref());
        }
    }

    mEndpoint = NetworkUtility::CreateEndpoint(profile);
}

RestService::RestService(std::shared_ptr<Endpoint> endpoint)
    : Service(endpoint)
{
    if (endpoint != nullptr && endpoint->GetProfile() != nullptr)
    {
        if (endpoint->GetProfile()->Dispatcher.cref() == nullptr)
        {
            mRestDispatcher = std::make_shared<RestDispatcher>();
            endpoint->GetProfile()->Dispatcher.set(mRestDispatcher);
        }
        else
        {
            mRestDispatcher = std::static_pointer_cast<RestDispatcher>(endpoint->GetProfile()->Dispatcher.cref());
        }
    }
}

RestService::~RestService()
{
}

bool RestService::Initialize()
{
    if (mRestDispatcher == nullptr)
    {
        return false;
    }

    return Service::Initialize();
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
