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
