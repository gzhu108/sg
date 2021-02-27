#include "RestService.h"
#include "NetworkUtility.h"

using namespace microreactor;


RestService::RestService(const std::string& hostName, uint16_t port)
{
    auto dispatcher = std::make_shared<RestDispatcher>();
    dispatcher->Address.set(hostName);
    dispatcher->Port.set(port);
    mListener = NetworkUtility::CreateListener(dispatcher);
}

RestService::RestService(std::shared_ptr<Profile> profile)
{
    auto dispatcher = std::make_shared<RestDispatcher>();

    if (profile == nullptr)
    {
        dispatcher->Address.set(ANY_HOST);
        dispatcher->Port.set(80);
    }
    else
    {
        dispatcher->Protocol.set(profile->Protocol.cref());
        dispatcher->Address.set(profile->Address.cref());
        dispatcher->Port.set(profile->Port.cref());
    }

    mListener = NetworkUtility::CreateListener(dispatcher);
}

RestService::RestService(std::shared_ptr<Listener> listener)
    : Service(listener)
{
    mListener->Dispatcher.set(std::make_shared<RestDispatcher>());
}

RestService::~RestService()
{
}
