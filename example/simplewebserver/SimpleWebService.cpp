#include "SimpleWebService.h"
#include "GetFileReactor.h"

using namespace sg::microreactor;
using namespace simplewebserver;


SimpleWebService::SimpleWebService(std::shared_ptr<Endpoint> endpoint)
    : RestService(endpoint)
{
}

SimpleWebService::~SimpleWebService()
{
}

bool SimpleWebService::Initialize()
{
    mRestDispatcher->RegisterRestReactorFactory("GET", "/", std::bind(&SimpleWebService::CreateGetFileReactor, this, std::placeholders::_1, std::placeholders::_2));
    
    return RestService::Initialize();
}

std::shared_ptr<Reactor> SimpleWebService::CreateGetFileReactor(std::shared_ptr<RestRequest> request, std::shared_ptr<Connection> connection)
{
    if (request->mUri.length() < std::string("/").length())
    {
        return nullptr;
    }

    auto reactor = std::make_shared<GetFileReactor>(connection, request);
    return reactor;
}
