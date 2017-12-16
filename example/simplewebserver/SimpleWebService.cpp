#include "SimpleWebService.h"
#include "GetFileFactory.h"

using namespace sg::microreactor;
using namespace simplewebserver;


SimpleWebService::SimpleWebService(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : RestService(endpoint, profile)
{
    RegisterRestFactory(std::make_shared<GetFileFactory>());
}

SimpleWebService::~SimpleWebService()
{
}
