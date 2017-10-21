#include "RestGetScene.h"
#include "StreetGangSessionManager.h"
#include "RequestErrorReactor.h"
#include "RequestGetSceneReactor.h"
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


RestGetScene::RestGetScene()
    : RestApi("POST", "/getscene", "ContentType: text/plain, application/json")
{
}

RestGetScene::~RestGetScene()
{
}

std::shared_ptr<Reactor> RestGetScene::CreateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < mPath.length() || request->mBody.mLength == 0)
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
        auto reactor = std::make_shared<RequestErrorReactor>(connection, errorResponse);
        reactor->SetMessageEncoder(streetGangResponseEncoder);
        return reactor;
    }

    auto reactor = std::make_shared<RequestGetSceneReactor>(connection, message);

    auto session = StreetGangSessionManager::GetInstance().GetSession(message->WorldId.cref());
    reactor->SetSession(session);
    reactor->SetMessageEncoder(streetGangResponseEncoder);

    return reactor;
}
