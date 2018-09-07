#include "RestService.h"

using namespace sg::microreactor;


RestService::RestService()
{
}

RestService::RestService(const std::string& hostName, uint16_t port)
    : mRestDispatcher(std::make_shared<RestDispatcher>())
{
    mProfile = std::make_shared<Profile>();
    mProfile->Protocol.set("tcp");
    mProfile->Address.set(hostName);
    mProfile->Port.set(port);
    mProfile->Dispatcher.set(mRestDispatcher);
}

RestService::RestService(std::shared_ptr<Profile> profile)
    : Microservice(profile)
{
    if (mProfile != nullptr)
    {
        if (mProfile->Dispatcher.cref() == nullptr)
        {
            mRestDispatcher = std::make_shared<RestDispatcher>();
            mProfile->Dispatcher.set(mRestDispatcher);
        }
        else
        {
            mRestDispatcher = std::static_pointer_cast<RestDispatcher>(mProfile->Dispatcher.cref());
        }
    }
}

RestService::RestService(std::shared_ptr<Endpoint> endpoint)
    : Microservice(endpoint)
{
    if (mProfile != nullptr)
    {
        if (mProfile->Dispatcher.cref() == nullptr)
        {
            mRestDispatcher = std::make_shared<RestDispatcher>();
            mProfile->Dispatcher.set(mRestDispatcher);
        }
        else
        {
            mRestDispatcher = std::static_pointer_cast<RestDispatcher>(mProfile->Dispatcher.cref());
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

    return Microservice::Initialize();
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
