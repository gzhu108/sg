#include "BouncerRestService.h"
#include "Microreactor.h"
#include "BouncerProfile.h"
#include "BouncerReactor.h"

#ifdef new
#undef new
#endif
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace sg::microreactor;
using namespace bouncer;


BouncerRestService::BouncerRestService(std::shared_ptr<Profile> profile)
    : RestService(profile)
{
}

BouncerRestService::~BouncerRestService()
{
}

bool BouncerRestService::Initialize()
{
    mRestDispatcher->RegisterRestReactorFactory("GET", "/", std::bind(&BouncerRestService::CreateBouncerReactor, this, std::placeholders::_1, std::placeholders::_2));
    mRestDispatcher->RegisterRestReactorFactory("POST", "/settings", std::bind(&BouncerRestService::CreateSettingsReactor, this, std::placeholders::_1, std::placeholders::_2));

    return RestService::Initialize();
}

std::shared_ptr<Reactor> BouncerRestService::CreateBouncerReactor(std::shared_ptr<RestRequest> request, std::shared_ptr<Connection> connection)
{
    if (request->mUri.length() < std::string("/").length())
    {
        RestResponse::SendErrorWith(*connection, 404, "Not found");
        return nullptr;
    }

    auto bouncerProfile = std::static_pointer_cast<BouncerProfile>(connection->GetProfile());
    std::string protocol = bouncerProfile->Protocol.cref();
    std::string targetName = bouncerProfile->TargetName.cref();
    uint16_t targetPort = bouncerProfile->TargetPort.cref();

    auto targetProfile = std::make_shared<Profile>();
    targetProfile->Protocol.set(protocol);
    targetProfile->Address.set(targetName);
    targetProfile->Port.set(targetPort);

    std::stringstream stream;
    stream << request->mMethod << " " << request->mUri << " " << request->mVersion << "\r\n";

    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "Endpoint")
        {
            stream << "Endpoint" << ": " << targetName << "\r\n";
        }
        else
        {
            stream << header.mName << ": " << header.mValue << "\r\n";
        }
    }

    stream << "\r\n";
    stream.write(request->mBody.mOffset, request->mBody.mLength);

    LOG("Send Request:\n%s\n", stream.str().c_str());

    std::shared_ptr<Connection> target = NetworkUtility::CreateConnection(targetProfile);
    return std::make_shared<BouncerReactor>(connection, target, stream);
}

std::shared_ptr<Reactor> BouncerRestService::CreateSettingsReactor(std::shared_ptr<RestRequest> request, std::shared_ptr<Connection> connection)
{
    if (request->mUri.length() < std::string("/settings").length() || request->mBody.mLength == 0)
    {
        return nullptr;
    }

    auto bouncerProfile = std::static_pointer_cast<BouncerProfile>(connection->GetProfile());
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
        ok.Send(*connection);
    }
    catch (...)
    {
        LOG("Failed to parse JSON: %.*s", (int)request->mBody.mLength, request->mBody.mOffset);
        RestResponse::SendErrorWith(*connection, 405, "Method Not Allowed");
    }

    return nullptr;
}
