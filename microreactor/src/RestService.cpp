#include "RestService.h"

using namespace sg::microreactor;


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
    , mRestDispatcher(std::make_shared<RestDispatcher>())
{
    if (mProfile != nullptr)
    {
        mProfile->Dispatcher.set(mRestDispatcher);
    }
}

RestService::RestService(std::shared_ptr<Endpoint> endpoint)
    : Microservice(endpoint)
    , mRestDispatcher(std::make_shared<RestDispatcher>())
{
    if (mProfile != nullptr)
    {
        mProfile->Dispatcher.set(mRestDispatcher);
    }
}

RestService::~RestService()
{
}

bool RestService::Initialize()
{
    return Microservice::Initialize();
}
