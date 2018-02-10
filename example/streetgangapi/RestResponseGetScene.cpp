#include "RestResponseGetScene.h"
#include "RestResponse.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


RestResponseGetScene::RestResponseGetScene()
{
}

RestResponseGetScene::~RestResponseGetScene()
{
}

bool RestResponseGetScene::Encode(std::ostream& stream) const
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    {
        writer.String("WorldId");
        writer.Uint64(WorldId.cref());

        writer.String("Rect");
        writer.StartObject();
        {
            writer.String("x");
            writer.Double(Rect->mX);
            writer.String("y");
            writer.Double(Rect->mY);
            writer.String("w");
            writer.Double(Rect->mW);
            writer.String("h");
            writer.Double(Rect->mH);
        }
        writer.EndObject();

        writer.String("Items");
        writer.StartArray();
        for (auto& item : Items.cref())
        {
            writer.StartObject();
            {
                writer.String("x");
                writer.Double(item.mX);
                writer.String("y");
                writer.Double(item.mY);
                writer.String("z");
                writer.Double(item.mZ);
            }
            writer.EndObject();
        }
        writer.EndArray();
    }
    writer.EndObject();

    RestResponse response;
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    response.mBody = buffer.GetString();

    return response.FlushToStream(stream);
}
