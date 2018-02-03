#include "MyServerService.h"
#include "GETv1versionReactor.h"
#include "POSTv1updateReactor.h"

using namespace sg::microreactor;
using namespace myserver;


MyServerService::MyServerService(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : RestService(endpoint, profile)
{
}

MyServerService::~MyServerService()
{
}

bool MyServerService::Initialize()
{
    mRestMessageDecoder->RegisterRestReactorFactory("GET", "/v1/version", std::bind(&MyServerService::CreateGETv1versionReactor, this, std::placeholders::_1, std::placeholders::_2));
    mRestMessageDecoder->RegisterRestReactorFactory("POST", "/v1/update", std::bind(&MyServerService::CreatePOSTv1updateReactor, this, std::placeholders::_1, std::placeholders::_2));
    
    return RestService::Initialize();
}

std::shared_ptr<Reactor> MyServerService::CreateGETv1versionReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < std::string("/v1/version").length())
    {
        return nullptr;
    }

    return std::make_shared<GETv1versionReactor>(connection, request);
}

std::shared_ptr<Reactor> MyServerService::CreatePOSTv1updateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < std::string("/v1/update").length())
    {
        return nullptr;
    }

    return std::make_shared<POSTv1updateReactor>(connection, request);
}

