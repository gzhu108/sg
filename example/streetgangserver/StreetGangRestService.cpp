#include "StreetGangRestService.h"
#include "RequestGetVersionReactor.h"
#include "RequestCreateWorldReactor.h"
#include "RequestGetSceneReactor.h"
#include "RestStreetGangResponder.h"
#include "RestRequestByebye.h"
#include "RestRequestGetVersion.h"
#include "RestRequestCreateWorld.h"
#include "RestRequestGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


StreetGangRestService::StreetGangRestService(std::shared_ptr<Endpoint> endpoint)
    : RestService(endpoint)
{
}

StreetGangRestService::~StreetGangRestService()
{
}

bool StreetGangRestService::Initialize()
{
    mRestDispatcher->RegisterRestReactorFactory("GET", "/version", std::bind(&StreetGangRestService::CreateGetVersionReactor, this, std::placeholders::_1, std::placeholders::_2));
    mRestDispatcher->RegisterRestReactorFactory("POST", "/createworld", std::bind(&StreetGangRestService::CreateCreateWorldReactor, this, std::placeholders::_1, std::placeholders::_2));
    mRestDispatcher->RegisterRestReactorFactory("POST", "/getscene", std::bind(&StreetGangRestService::CreateGetSceneReactor, this, std::placeholders::_1, std::placeholders::_2));
    
    return RestService::Initialize();
}

std::shared_ptr<Reactor> StreetGangRestService::CreateGetVersionReactor(std::shared_ptr<RestMessage> message, std::shared_ptr<Connection> connection)
{
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request == nullptr || request->mUri.length() != std::string("/version").length())
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid request");
        return nullptr;
    }

    auto restRequestGetVersion = std::make_shared<RestRequestGetVersion>();
    if (restRequestGetVersion->Decode(request))
    {
        return std::make_shared<RequestGetVersionReactor>(connection, restRequestGetVersion, std::make_shared<RestStreetGangResponder>(connection));
    }
    else
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid JSON");
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangRestService::CreateCreateWorldReactor(std::shared_ptr<RestMessage> message, std::shared_ptr<Connection> connection)
{
    std::string path = "/createworld";
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request == nullptr || request->mUri.length() < path.length())
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid request");
        return nullptr;
    }

    auto restRequestCreateWorld = std::make_shared<RestRequestCreateWorld>();
    if (restRequestCreateWorld->Decode(request))
    {
        return std::make_shared<RequestCreateWorldReactor>(connection, restRequestCreateWorld, std::make_shared<RestStreetGangResponder>(connection));
    }
    else
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid JSON");
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangRestService::CreateGetSceneReactor(std::shared_ptr<RestMessage> message, std::shared_ptr<Connection> connection)
{
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request == nullptr || request->mUri.length() < std::string("/getscene").length())
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid request");
        return nullptr;
    }

    bool correctContentType = false;
    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "Content-Type" && header.mValue == "application/json")
        {
            correctContentType = true;
            break;
        }
    }

    if (!correctContentType)
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid content type");
        return nullptr;
    }

    auto restRequestGetScene = std::make_shared<RestRequestGetScene>();
    if (restRequestGetScene->Decode(request))
    {
        return std::make_shared<RequestGetSceneReactor>(connection, restRequestGetScene, std::make_shared<RestStreetGangResponder>(connection));
    }
    else
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid JSON");
        return nullptr;
    }

    return nullptr;
}
