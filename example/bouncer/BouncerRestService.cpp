#include "BouncerRestService.h"
#include "Microreactor.h"
#include "BouncerDispatcher.h"
#include "BouncerReactor.h"

#ifdef new
#undef new
#endif
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace microreactor;
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
    mListener->GetDispatcher<RestDispatcher>()->RegisterResource("GET", "/", std::bind(&BouncerRestService::CreateBouncerReactor, this, std::placeholders::_1, std::placeholders::_2));
    mListener->GetDispatcher<RestDispatcher>()->RegisterResource("POST", "/settings", std::bind(&BouncerRestService::CreateSettingsReactor, this, std::placeholders::_1, std::placeholders::_2));

    return RestService::Initialize();
}

std::shared_ptr<Reactor> BouncerRestService::CreateBouncerReactor(std::shared_ptr<RestMessage> message, Connection& connection)
{
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request->mUri.length() < std::string("/").length())
    {
        RestResponse::SendErrorWith(connection, 404, "Not found");
        return nullptr;
    }

    auto bouncerDispatcher = mListener->GetDispatcher<BouncerDispatcher>();
    std::string protocol = bouncerDispatcher->Protocol.cref();
    std::string targetName = bouncerDispatcher->TargetName.cref();
    uint16_t targetPort = bouncerDispatcher->TargetPort.cref();

    auto targetDispatcher = std::make_shared<Dispatcher>();
    targetDispatcher->Protocol.set(protocol);
    targetDispatcher->Address.set(targetName);
    targetDispatcher->Port.set(targetPort);

    std::stringstream stream;
    stream << request->mMethod << " " << request->mUri << " " << request->mVersion << "\r\n";

    for (const auto& header : request->mHeaders)
    {
        if (header.mName == "Listener")
        {
            stream << "Listener" << ": " << targetName << "\r\n";
        }
        else
        {
            stream << header.mName << ": " << header.mValue << "\r\n";
        }
    }

    stream << "\r\n";
    stream.write(request->mBody.mOffset, request->mBody.mLength);

    LOG("Write Request:\n%s\n", stream.str().c_str());

    std::shared_ptr<Connection> target = NetworkUtility::CreateConnection(targetDispatcher);
    return std::make_shared<BouncerReactor>(connection, target, stream);
}

std::shared_ptr<Reactor> BouncerRestService::CreateSettingsReactor(std::shared_ptr<RestMessage> message, Connection& connection)
{
    auto request = std::static_pointer_cast<RestRequest>(message);
    if (request->mUri.length() < std::string("/settings").length() || request->mBody.mLength == 0)
    {
        return nullptr;
    }

    auto bouncerDispatcher = connection.GetDispatcher<BouncerDispatcher>();
    std::string targetName = bouncerDispatcher->TargetName.cref();
    uint16_t targetPort = bouncerDispatcher->TargetPort.cref();

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
        bouncerDispatcher->TargetName.set(targetName);
        bouncerDispatcher->TargetPort.set(targetPort);

        // Respond with OK
        RestResponse ok;
        ok.Send(connection);
    }
    catch (...)
    {
        LOG("Failed to parse JSON: %.*s", (int)request->mBody.mLength, request->mBody.mOffset);
        RestResponse::SendErrorWith(connection, 405, "Method Not Allowed");
    }

    return nullptr;
}
