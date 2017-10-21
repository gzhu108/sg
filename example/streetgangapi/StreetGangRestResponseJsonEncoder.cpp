#include "StreetGangRestResponseJsonEncoder.h"
#include "Serializer.h"
#include "RestResponse.h"

#ifdef new
#undef new
#endif
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


StreetGangRestResponseJsonEncoder::StreetGangRestResponseJsonEncoder()
{
}

StreetGangRestResponseJsonEncoder::~StreetGangRestResponseJsonEncoder()
{
}

bool StreetGangRestResponseJsonEncoder::EncodeMessage(std::shared_ptr<ResponseError> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    {
        writer.String("Error");
        writer.StartObject();
        {
            writer.String("RequestId");
            writer.Int(message->RequestId.cref());
            writer.String("Error message");
            writer.String(message->ErrorMessage->c_str());
        }
        writer.EndObject();
    }
    writer.EndObject();

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 500;
    response.mStatusText = "Interanl Server Error";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    response.mBody = buffer.GetString();

    return response.FlushToStream(stream);
}

bool StreetGangRestResponseJsonEncoder::EncodeMessage(std::shared_ptr<ResponseGetVersion> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    {
        writer.String("Version");
        writer.String(message->Version->c_str());
    }
    writer.EndObject();

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 200;
    response.mStatusText = "OK";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    response.mBody = buffer.GetString();

    return response.FlushToStream(stream);
}

bool StreetGangRestResponseJsonEncoder::EncodeMessage(std::shared_ptr<ResponseCreateWorld> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    {
        writer.String("WorldId");
        writer.Uint64(message->WorldId.cref());
        writer.String("WorldName");
        writer.String(message->WorldName->c_str());
    }
    writer.EndObject();

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 200;
    response.mStatusText = "OK";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    response.mBody = buffer.GetString();

    return response.FlushToStream(stream);
}

bool StreetGangRestResponseJsonEncoder::EncodeMessage(std::shared_ptr<ResponseGetScene> message, std::ostream& stream)
{
    if (message == nullptr)
    {
        return false;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    
    writer.StartObject();
    {
        writer.String("WorldId");
        writer.Uint64(message->WorldId.cref());

        writer.String("Rect");
        writer.StartObject();
        {
            writer.String("x");
            writer.Double(message->Rect->mX);
            writer.String("y");
            writer.Double(message->Rect->mY);
            writer.String("w");
            writer.Double(message->Rect->mW);
            writer.String("h");
            writer.Double(message->Rect->mH);
        }
        writer.EndObject();

        writer.String("Items");
        writer.StartArray();
        for (size_t i = 0; i < message->Items->size(); i++)
        {
            writer.StartObject();
            {
                writer.String("x");
                writer.Double(message->Items->at(i).mX);
                writer.String("y");
                writer.Double(message->Items->at(i).mY);
                writer.String("z");
                writer.Double(message->Items->at(i).mZ);
            }
            writer.EndObject();
        }
        writer.EndArray();
    }
    writer.EndObject();

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 200;
    response.mStatusText = "OK";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    response.mBody = buffer.GetString();

    return response.FlushToStream(stream);
}
