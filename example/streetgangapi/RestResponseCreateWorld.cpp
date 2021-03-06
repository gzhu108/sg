#include "RestResponseCreateWorld.h"
#include "RestResponse.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace microreactor;
using namespace streetgangapi;


RestResponseCreateWorld::RestResponseCreateWorld()
{
}

RestResponseCreateWorld::~RestResponseCreateWorld()
{
}

bool RestResponseCreateWorld::Encode(std::ostream& stream) const
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    {
        writer.String("WorldId");
        writer.Uint64(WorldId.cref());
        writer.String("WorldName");
        writer.String(WorldName->c_str());
    }
    writer.EndObject();

    RestResponse response;
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    SetHttpBody(buffer.GetString(), response);

    return response.Encode(stream);
}
