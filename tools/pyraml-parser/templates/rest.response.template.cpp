#include "$method$funcResponse.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace $namespace;


$method$funcResponse::$method$funcResponse()
{
}

$method$funcResponse::~$method$funcResponse()
{
}

bool $method$funcResponse::FlushToBuffer(std::string& buffer)
{
    rapidjson::StringBuffer jsonbuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(jsonbuffer);

    writer.StartObject();

    @property_begin
    writer.String("$propname");
    writer.String($propname->c_str());

    @property_end
    writer.EndObject();

    mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    mBody = jsonbuffer.GetString();

    return RestResponse::FlushToBuffer(buffer);
}
