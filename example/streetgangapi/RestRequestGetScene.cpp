#include "RestRequestGetScene.h"

#ifdef new
#undef new
#endif
#define RAPIDJSON_ASSERT(x) ((void)0)

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


RestRequestGetScene::RestRequestGetScene()
{
}

RestRequestGetScene::~RestRequestGetScene()
{
}

bool RestRequestGetScene::Decode(std::shared_ptr<sg::microreactor::RestRequest> request)
{
    if (request == nullptr || request->mBody.mLength == 0)
    {
        return false;
    }

    // Parse the json
    //
    //    {
    //        "WorldId":1234,
    //        "Rect":
    //        {
    //            "x":0.4,
    //            "y":0.4,
    //            "w":0.2,
    //            "h":0.2
    //        }
    //    }

    try
    {
        rapidjson::Document doc;
        doc.Parse(request->mBody.mOffset, request->mBody.mLength);
        if (doc.IsNull())
        {
            return false;
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        LOG("RestGetScene() JSON = %s", buffer.GetString());

        rapidjson::Value& jsonWorldId = doc["WorldId"];
        if (jsonWorldId.IsNull())
        {
            return false;
        }
        else
        {
            WorldId.set(jsonWorldId.Get<streetgangapi::SessionId>());
        }

        rapidjson::Value& jsonRect = doc["Rect"];
        if (jsonRect.IsNull())
        {
            return false;
        }
        else
        {
            rapidjson::Value& jsonX = jsonRect["x"];
            if (jsonX.IsNull())
            {
                return false;
            }
            else
            {
                Rect->mX = jsonX.Get<float>();
            }

            rapidjson::Value& jsonY = jsonRect["y"];
            if (jsonY.IsNull())
            {
                return false;
            }
            else
            {
                Rect->mY = jsonY.Get<float>();
            }

            rapidjson::Value& jsonW = jsonRect["w"];
            if (jsonW.IsNull())
            {
                return false;
            }
            else
            {
                Rect->mW = jsonW.Get<float>();
            }

            rapidjson::Value& jsonH = jsonRect["h"];
            if (jsonH.IsNull())
            {
                return false;
            }
            else
            {
                Rect->mH = jsonH.Get<float>();
            }
        }
    }
    catch (...)
    {
        LOG("Failed to parse JSON: %.*s", (int)request->mBody.mLength, request->mBody.mOffset);
        return false;
    }

    return true;
}
