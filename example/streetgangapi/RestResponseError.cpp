#include "RestResponseError.h"
#include "RestResponse.h"

#ifdef new
#undef new
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace streetgangapi;


RestResponseError::RestResponseError()
{
}

RestResponseError::~RestResponseError()
{
}

bool RestResponseError::Encode(std::ostream& stream) const
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    {
        writer.String("Error");
        writer.StartObject();
        {
            writer.String("RequestId");
            writer.Int(RequestId.cref());
            writer.String("Error message");
            writer.String(ErrorMessage->c_str());
        }
        writer.EndObject();
    }
    writer.EndObject();

    RestResponse response;
    response.mVersion = "HTTP/1.1";
    response.mStatusCode = 500;
    response.mStatusText = "Interanl Server Error";
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));
    SetHttpBody(buffer.GetString(), response);

    return response.Encode(stream);
}
