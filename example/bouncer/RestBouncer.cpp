#include "RestBouncer.h"
#include "NetworkUtility.h"
#include "RestResponse.h"
#include "BouncerProfile.h"
#include "BouncerReactor.h"

using namespace sg::microreactor;
using namespace bouncer;


RestBouncer::RestBouncer()
    : RestApi("GET", "/", "ContentType: */*")
{
}

RestBouncer::~RestBouncer()
{
}

std::shared_ptr<Reactor> RestBouncer::CreateReactor(std::shared_ptr<RestRequest> request, Connection& connection)
{
    if (request->mUri.length() < mPath.length())
    {
        RespondWithError(connection, 404, "Not found");
        return nullptr;
    }

    auto bouncerProfile = std::static_pointer_cast<BouncerProfile>(connection.GetProfile());
    std::string protocol = bouncerProfile->Protocol.cref();
    std::string targetName = bouncerProfile->TargetName.cref();
    uint16_t targetPort = bouncerProfile->TargetPort.cref();

    auto targetProfile = std::make_shared<Profile>();
    targetProfile->Protocol.set(protocol);
    targetProfile->Address.set(targetName);
    targetProfile->Port.set(targetPort);

    std::stringstream stream;
/*
std::shared_ptr<std::string> mRawMessage;
        std::string mMethod;
        std::string mUri;
        std::string mVersion;
        std::vector<HttpHeader> mHeaders;
        HttpData mBody;
        std::vector<HttpChunk> mChunks;
*/

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

void RestBouncer::RespondWithError(Connection& connection, uint32_t statusCode, const std::string& statusText)
{
    RestResponse response;
    response.mStatusCode = statusCode;
    response.mStatusText = statusText;
    response.Send(connection);
}
