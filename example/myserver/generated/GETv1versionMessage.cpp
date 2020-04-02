#include "GETv1versionMessage.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace myserver;


GETv1versionMessage::GETv1versionMessage(std::shared_ptr<RestRequest> request)
{
    // TODO: parse HTTP request here
}

GETv1versionMessage::~GETv1versionMessage()
{
}

bool GETv1versionMessage::Write(std::string& buffer) const
{
    rapidjson::StringBuffer jsonbuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(jsonbuffer);

    writer.StartObject();
    {
        writer.String("Version");
        writer.String(Version->c_str());
    }
    writer.EndObject();

    buffer = jsonbuffer.GetString();
    return true;
}
