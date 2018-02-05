#include "MyServerServiceBase.h"
#include "GETv1versionReactorBase.h"
#include "POSTv1updateReactorBase.h"

using namespace sg::microreactor;
using namespace myserver;


MyServerServiceBase::MyServerServiceBase(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : RestService(endpoint, profile)
{
}

MyServerServiceBase::~MyServerServiceBase()
{
}

bool MyServerServiceBase::Initialize()
{
    mRestMessageDecoder->RegisterRestReactorFactory("GET", "/v1/version", std::bind(&MyServerServiceBase::CreateGETv1versionReactor, this, std::placeholders::_1, std::placeholders::_2));
    mRestMessageDecoder->RegisterRestReactorFactory("POST", "/v1/update", std::bind(&MyServerServiceBase::CreatePOSTv1updateReactor, this, std::placeholders::_1, std::placeholders::_2));
    
    return RestService::Initialize();
}

// Override CreateGETv1versionReactor() in the derived class
std::shared_ptr<Reactor> MyServerServiceBase::CreateGETv1versionReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    return std::make_shared<GETv1versionReactorBase>(connection, request);
}

// Override CreatePOSTv1updateReactor() in the derived class
std::shared_ptr<Reactor> MyServerServiceBase::CreatePOSTv1updateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    return std::make_shared<POSTv1updateReactorBase>(connection, request);
}

