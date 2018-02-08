#include "StreetGangRestService.h"
#include "RequestGetVersionReactor.h"
#include "RequestCreateWorldReactor.h"
#include "RequestGetSceneReactor.h"
#include "StreetGangSessionManager.h"
#include "RequestErrorReactor.h"
#include "StreetGangRestResponseTextEncoder.h"
#include "StreetGangRestResponseJsonEncoder.h"

#ifdef new
#undef new
#endif
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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

std::shared_ptr<sg::microreactor::Reactor> StreetGangRestService::CreateGetVersionReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection)
{
    if (request->mUri.length() != std::string("/version").length())
    {
        return nullptr;
    }

    auto message = std::make_shared<RequestGetVersion>();
    std::shared_ptr<StreetGangResponseEncoder> streetGangResponseEncoder;

    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "Accept" && header.mValue == "application/json")
        {
            streetGangResponseEncoder = std::make_shared<StreetGangRestResponseJsonEncoder>();
            break;
        }
    }

    if (streetGangResponseEncoder == nullptr)
    {
        // default content type is plain text
        streetGangResponseEncoder = std::make_shared<StreetGangRestResponseTextEncoder>();
    }

    auto reactor = std::make_shared<RequestGetVersionReactor>(connection, message, nullptr);
    reactor->SetMessageEncoder(streetGangResponseEncoder);
    return reactor;
}

std::shared_ptr<sg::microreactor::Reactor> StreetGangRestService::CreateCreateWorldReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection)
{
    std::string path = "/createworld";
    if (request->mUri.length() < path.length())
    {
        return nullptr;
    }

    auto message = std::make_shared<RequestCreateWorld>();
    if (request->mUri.length() > path.length())
    {
        std::string worldName = request->mUri.substr(path.length() + 1, std::string::npos);
        LOG("RestCreateWorld::CreateReactor() [WorldName=%s]", worldName.c_str());
        message->WorldName.set(worldName);
    }

    std::shared_ptr<StreetGangResponseEncoder> streetGangResponseEncoder;
    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "Accept" && header.mValue == "application/json")
        {
            streetGangResponseEncoder = std::make_shared<StreetGangRestResponseJsonEncoder>();
            break;
        }
    }

    if (streetGangResponseEncoder == nullptr)
    {
        // default content type is plain text
        streetGangResponseEncoder = std::make_shared<StreetGangRestResponseTextEncoder>();
    }

    auto reactor = std::make_shared<RequestCreateWorldReactor>(connection, message, nullptr);
    reactor->SetMessageEncoder(streetGangResponseEncoder);

    return reactor;
}

std::shared_ptr<sg::microreactor::Reactor> StreetGangRestService::CreateGetSceneReactor(std::shared_ptr<sg::microreactor::RestRequest> request, sg::microreactor::Connection& connection)
{
    if (request->mUri.length() < std::string("/getscene").length() || request->mBody.mLength == 0)
    {
        return nullptr;
    }

    auto message = std::make_shared<RequestGetScene>();
    std::shared_ptr<StreetGangResponseEncoder> streetGangResponseEncoder;

    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "Accept" && header.mValue == "application/json")
        {
            streetGangResponseEncoder = std::make_shared<StreetGangRestResponseJsonEncoder>();
            break;
        }
    }

    if (streetGangResponseEncoder == nullptr)
    {
        // default content type is plain text
        streetGangResponseEncoder = std::make_shared<StreetGangRestResponseTextEncoder>();
    }

    // Parse the json
    //{
    //    "GetScene":
    //    {
    //        "WorldId":1234
    //        "Rect":
    //        {
    //            "x":0.4
    //            "y":0.4
    //            "w":0.2
    //            "z":0.2
    //        }
    //    }
    //}

    try
    {
        rapidjson::Document doc;
        doc.Parse(request->mBody.mOffset, request->mBody.mLength);
        if (doc.IsNull())
        {
            return nullptr;
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        LOG("RestGetScene() JSON = %s", buffer.GetString());

        rapidjson::Value& jsonGetScene = doc["GetScene"];
        if (jsonGetScene.IsNull())
        {
            return nullptr;
        }
        else
        {
            rapidjson::Value& jsonWorldId = jsonGetScene["WorldId"];
            if (jsonWorldId.IsNull())
            {
                return nullptr;
            }
            else
            {
                message->WorldId.set(jsonWorldId.Get<streetgangapi::SessionId>());
            }

            rapidjson::Value& jsonRect = jsonGetScene["Rect"];
            if (jsonRect.IsNull())
            {
                return nullptr;
            }
            else
            {
                rapidjson::Value& jsonX = jsonRect["x"];
                if (jsonX.IsNull())
                {
                    return nullptr;
                }
                else
                {
                    message->Rect->mX = jsonX.Get<float>();
                }

                rapidjson::Value& jsonY = jsonRect["y"];
                if (jsonY.IsNull())
                {
                    return nullptr;
                }
                else
                {
                    message->Rect->mY = jsonY.Get<float>();
                }

                rapidjson::Value& jsonW = jsonRect["w"];
                if (jsonW.IsNull())
                {
                    return nullptr;
                }
                else
                {
                    message->Rect->mW = jsonW.Get<float>();
                }

                rapidjson::Value& jsonH = jsonRect["h"];
                if (jsonH.IsNull())
                {
                    return nullptr;
                }
                else
                {
                    message->Rect->mH = jsonH.Get<float>();
                }
            }
        }
    }
    catch (...)
    {
        LOG("Failed to parse JSON: %.*s", (int)request->mBody.mLength, request->mBody.mOffset);
        auto errorResponse = std::make_shared<ResponseError>();
        errorResponse->Result.set((int32_t)ResultCode::ErrorBadRequest);
        errorResponse->RequestId.set(message->Id.cref());
        errorResponse->ErrorMessage.set("Failed to parse JSON");
        auto reactor = std::make_shared<RequestErrorReactor>(connection, errorResponse, nullptr);
        reactor->SetMessageEncoder(streetGangResponseEncoder);
        return reactor;
    }

    auto reactor = std::make_shared<RequestGetSceneReactor>(connection, message, nullptr);

    auto session = StreetGangSessionManager::GetInstance().GetSession(message->WorldId.cref());
    reactor->SetSession(session);
    reactor->SetMessageEncoder(streetGangResponseEncoder);

    return reactor;
}
