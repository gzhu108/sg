#include "StreetGangRestService.h"
#include "RequestGetVersionReactor.h"
#include "RequestCreateWorldReactor.h"
#include "RequestGetSceneReactor.h"
#include "RestStreetGangResponder.h"
#include "RestRequestByebye.h"
#include "RestRequestGetVersion.h"
#include "RestRequestCreateWorld.h"
#include "RestRequestGetScene.h"
#include "StreetGangSessionManager.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


StreetGangRestService::StreetGangRestService(std::shared_ptr<Endpoint> endpoint, std::shared_ptr<Profile> profile)
    : RestService(endpoint, profile)
{
}

StreetGangRestService::~StreetGangRestService()
{
}

bool StreetGangRestService::Initialize()
{
    mRestMessageDecoder->RegisterRestReactorFactory("GET", "/version", std::bind(&StreetGangRestService::CreateGetVersionReactor, this, std::placeholders::_1, std::placeholders::_2));
    mRestMessageDecoder->RegisterRestReactorFactory("POST", "/createworld", std::bind(&StreetGangRestService::CreateCreateWorldReactor, this, std::placeholders::_1, std::placeholders::_2));
    mRestMessageDecoder->RegisterRestReactorFactory("POST", "/getscene", std::bind(&StreetGangRestService::CreateGetSceneReactor, this, std::placeholders::_1, std::placeholders::_2));
    
    return RestService::Initialize();
}

std::shared_ptr<Reactor> StreetGangRestService::CreateGetVersionReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request == nullptr || request->mUri.length() != std::string("/version").length())
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid request");
        return nullptr;
    }

    auto message = std::make_shared<RestRequestGetVersion>();
    if (message->Decode(request))
    {
        return std::make_shared<RequestGetVersionReactor>(connection, message, std::make_shared<RestStreetGangResponder>(connection));
    }
    else
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid JSON");
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangRestService::CreateCreateWorldReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    std::string path = "/createworld";
    if (request == nullptr || request->mUri.length() < path.length())
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid request");
        return nullptr;
    }

    auto message = std::make_shared<RestRequestCreateWorld>();
    if (message->Decode(request))
    {
        return std::make_shared<RequestCreateWorldReactor>(connection, message, std::make_shared<RestStreetGangResponder>(connection));
    }
    else
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid JSON");
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Reactor> StreetGangRestService::CreateGetSceneReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
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

    auto message = std::make_shared<RestRequestGetScene>();
    if (message->Decode(request))
    {
        auto reactor = std::make_shared<RequestGetSceneReactor>(connection, message, std::make_shared<RestStreetGangResponder>(connection));
        auto session = StreetGangSessionManager::GetInstance().GetSession(message->WorldId.cref());
        reactor->SetSession(session);
        return reactor;
    }
    else
    {
        auto responder = std::make_shared<RestStreetGangResponder>(connection);
        responder->SendErrorResponse("Unknown", ResultCode::ErrorBadRequest, static_cast<int32_t>(streetgangapi::ID::Unknown), "Invalid JSON");
        return nullptr;
    }

    return nullptr;
}
