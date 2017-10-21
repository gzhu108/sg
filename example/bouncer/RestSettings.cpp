#include "RestSettings.h"
#include "RestResponse.h"
#include "BouncerProfile.h"

#ifdef new
#undef new
#endif
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace bouncer;


RestSettings::RestSettings()
    : RestApi("POST", "/settings", "ContentType: application/json")
{
}

RestSettings::~RestSettings()
{
}

std::shared_ptr<Reactor> RestSettings::CreateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < mPath.length() || request->mBody.mLength == 0)
    {
        return nullptr;
    }

    auto bouncerProfile = std::static_pointer_cast<BouncerProfile>(connection.GetProfile());
    std::string targetName = bouncerProfile->TargetName.cref();
    uint16_t targetPort = bouncerProfile->TargetPort.cref();

    try
    {
        rapidjson::Document doc;
        doc.Parse(request->mBody.mOffset, request->mBody.mLength);
        if (doc.IsNull())
        {
            return nullptr;
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        LOG("RestGetScene() JSON = %s", buffer.GetString());

        rapidjson::Value& jsonTargetName = doc["TargetName"];
        if (!jsonTargetName.IsNull())
        {
            targetName = jsonTargetName.GetString();
        }

        rapidjson::Value& jsonTargetPort = doc["TargetPort"];
        if (!jsonTargetPort.IsNull())
        {
            targetPort = jsonTargetPort.GetUint();
        }

        // Update the target name and port
        bouncerProfile->TargetName.set(targetName);
        bouncerProfile->TargetPort.set(targetPort);

        // Respond with OK
        RestResponse ok;
        ok.Send(connection);
    }
    catch (...)
    {
        LOG("Failed to parse JSON: %.*s", (int)request->mBody.mLength, request->mBody.mOffset);
        RespondWithError(connection, 405, "Method Not Allowed");
    }

    return nullptr;
}

void RestSettings::RespondWithError(Connection& connection, uint32_t statusCode, const std::string& statusText)
{
    RestResponse response;
    response.mStatusCode = statusCode;
    response.mStatusText = statusText;
    response.Send(connection);
}
