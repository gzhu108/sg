#include "POSTv1updateMessage.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace myserver;


POSTv1updateMessage::POSTv1updateMessage(std::shared_ptr<RestRequest> request)
{
    // TODO: parse HTTP request here
}

POSTv1updateMessage::~POSTv1updateMessage()
{
}

bool POSTv1updateMessage::Write(std::string& buffer) const
{
    rapidjson::StringBuffer jsonbuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(jsonbuffer);

    writer.StartObject();
    {
        writer.String("Name");
        writer.String(Name->c_str());

        writer.String("Age");
        writer.String(Age->c_str());
    }
    writer.EndObject();

    buffer = jsonbuffer.GetString();
    return true;
}
