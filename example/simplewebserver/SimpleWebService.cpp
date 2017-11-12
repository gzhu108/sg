#include "SimpleWebService.h"
#include "GetFileApi.h"

using namespace sg::microreactor;
using namespace simplewebserver;


SimpleWebService::SimpleWebService(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : RestService(endpoint, profile)
{
    RegisterRestApi(std::make_shared<GetFileApi>());
}

SimpleWebService::~SimpleWebService()
{
}
