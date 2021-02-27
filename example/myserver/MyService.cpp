#include "MyService.h"
#include "VersionReactor.h"
#include "UpdateReactor.h"

using namespace myserver;


MyService::MyService(std::shared_ptr<microreactor::Profile> profile)
    : MyServerServiceBase(profile)
{
}

MyService::~MyService()
{
}

std::shared_ptr<microreactor::Reactor> MyService::CreateGETv1versionReactor(std::shared_ptr<microreactor::RestMessage> request, microreactor::Connection& connection)
{
    auto restRequest = std::static_pointer_cast<microreactor::RestRequest>(request);

    if (restRequest->mUri.length() < std::string("/v1/version").length())
    {
        return nullptr;
    }

    return std::make_shared<VersionReactor>(connection, restRequest);
}

std::shared_ptr<microreactor::Reactor> MyService::CreatePOSTv1updateReactor(std::shared_ptr<microreactor::RestMessage> request, microreactor::Connection& connection)
{
    auto restRequest = std::static_pointer_cast<microreactor::RestRequest>(request);

    if (restRequest->mUri.length() < std::string("/v1/update").length())
    {
        return nullptr;
    }

    return std::make_shared<UpdateReactor>(connection, restRequest);
}
