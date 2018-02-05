#include "MyService.h"
#include "VersionReactor.h"
#include "UpdateReactor.h"

using namespace sg::microreactor;
using namespace myserver;


MyService::MyService(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : MyServerServiceBase(endpoint, profile)
{
}

MyService::~MyService()
{
}

std::shared_ptr<Reactor> MyService::CreateGETv1versionReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < std::string("/v1/version").length())
    {
        return nullptr;
    }

    return std::make_shared<VersionReactor>(connection, request);
}

//std::shared_ptr<Reactor> MyService::CreatePOSTv1updateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
//{
//    if (request->mUri.length() < std::string("/v1/update").length())
//    {
//        return nullptr;
//    }
//
//    return std::make_shared<UpdateReactor>(connection, request);
//}
