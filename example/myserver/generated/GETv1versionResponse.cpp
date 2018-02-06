#include "GETv1versionResponse.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace myserver;


GETv1versionResponse::GETv1versionResponse()
{
}

GETv1versionResponse::~GETv1versionResponse()
{
}

bool GETv1versionResponse::FlushToBuffer(std::string& buffer)
{
    rapidjson::StringBuffer jsonbuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(jsonbuffer);

    writer.StartObject();

    writer.String("Version");
    writer.String(Version->c_str());

    writer.EndObject();

    mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    mBody = jsonbuffer.GetString();

    return RestResponse::FlushToBuffer(buffer);
}
