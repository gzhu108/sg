#include "POSTv1updateResponse.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace myserver;


POSTv1updateResponse::POSTv1updateResponse()
{
}

POSTv1updateResponse::~POSTv1updateResponse()
{
}

bool POSTv1updateResponse::FlushToBuffer(std::string& buffer)
{
    rapidjson::StringBuffer jsonbuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(jsonbuffer);

    writer.StartObject();

    writer.EndObject();

    mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    mBody = jsonbuffer.GetString();

    return RestResponse::FlushToBuffer(buffer);
}
