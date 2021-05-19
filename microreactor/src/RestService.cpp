#include "RestService.h"
#include "NetworkUtility.h"

using namespace microreactor;


RestService::RestService(const std::string& hostName, uint16_t port)
{
    auto dispatcher = std::make_shared<RestDispatcher>();
    dispatcher->Address(hostName);
    dispatcher->Port(port);
    mListener = NetworkUtility::CreateListener(dispatcher);
}

RestService::RestService(std::shared_ptr<Profile> profile)
{
    auto dispatcher = std::make_shared<RestDispatcher>();

    if (profile == nullptr)
    {
        dispatcher->Address(ANY_HOST);
        dispatcher->Port(80);
    }
    else
    {
        dispatcher->Protocol(profile->Protocol());
        dispatcher->Address(profile->Address());
        dispatcher->Port(profile->Port());
    }

    mListener = NetworkUtility::CreateListener(dispatcher);
}

RestService::RestService(std::shared_ptr<Listener> listener)
    : Service(listener)
{
    mListener->Dispatcher(std::make_shared<RestDispatcher>());
}

RestService::~RestService()
{
}
