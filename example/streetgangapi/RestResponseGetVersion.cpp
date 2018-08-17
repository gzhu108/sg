#include "RestResponseGetVersion.h"
#include "RestResponse.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


RestResponseGetVersion::RestResponseGetVersion()
{
}

RestResponseGetVersion::~RestResponseGetVersion()
{
}

bool RestResponseGetVersion::Encode(std::ostream& stream) const
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    {
        writer.String("Version");
        writer.String(Version->c_str());
    }
    writer.EndObject();

    RestResponse response;
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    SetHttpBody(buffer.GetString(), response);

    return response.FlushToStream(stream);
}
