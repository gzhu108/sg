#include "SimpleWebService.h"
#include "GetFileReactor.h"

using namespace sg::microreactor;
using namespace simplewebserver;


SimpleWebService::SimpleWebService(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : RestService(endpoint, profile)
{
}

SimpleWebService::~SimpleWebService()
{
}

bool SimpleWebService::Initialize()
{
    mRestMessageDecoder->RegisterRestReactorFactory("GET", "/", std::bind(&SimpleWebService::CreateGetFileReactor, this, std::placeholders::_1, std::placeholders::_2));
    
    return RestService::Initialize();
}

std::shared_ptr<Reactor> SimpleWebService::CreateGetFileReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < std::string("/").length())
    {
        return nullptr;
    }

    auto reactor = std::make_shared<GetFileReactor>(connection, request);
    return reactor;
}
