#include "RestService.h"

using namespace sg::microreactor;


RestService::RestService(const std::string& hostName, uint16_t port)
    : mRestMessageDecoder(std::make_shared<RestMessageDecoder>())
{
    mProfile = std::make_shared<Profile>();
    mProfile->Protocol.set("tcp");
    mProfile->Address.set(hostName);
    mProfile->Port.set(port);
    mProfile->Dispatcher.set(mRestMessageDecoder);
}

RestService::RestService(std::shared_ptr<Profile> profile)
    : Microservice(profile)
    , mRestMessageDecoder(std::make_shared<RestMessageDecoder>())
{
    if (mProfile != nullptr)
    {
        mProfile->Dispatcher.set(mRestMessageDecoder);
    }
}

RestService::RestService(std::shared_ptr<Host> host, std::shared_ptr<Profile> profile)
    : Microservice(host, profile)
    , mRestMessageDecoder(std::make_shared<RestMessageDecoder>())
{
    if (mProfile != nullptr)
    {
        mProfile->Dispatcher.set(mRestMessageDecoder);
    }
}

RestService::~RestService()
{
}

void RestService::RegisterRestApi(std::shared_ptr<RestApi> restApi)
{ 
    return mRestMessageDecoder->RegisterRestApi(restApi);
}
