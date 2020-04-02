#include "VersionReactor.h"
#include "generated/GETv1versionMessage.h"

using namespace sg::microreactor;
using namespace myserver;


VersionReactor::VersionReactor(Connection& connection, std::shared_ptr<RestRequest> request)
    : GETv1versionReactorBase(connection, request)
{
}

VersionReactor::~VersionReactor()
{
}

bool VersionReactor::Process()
{
    if (InputMessage() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return sg::microreactor::RestResponse::SendErrorWith(mConnection, 400, "GET [/v1/version] Invalid request");
    }

    RestResponse response;
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));

    return response.Send(mConnection, GETv1versionMessage(InputMessage()));
}
