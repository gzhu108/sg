#include "VersionReactor.h"
#include "GETv1versionResponse.h"

using namespace sg::microreactor;
using namespace myserver;


VersionReactor::VersionReactor(Connection& connection, std::shared_ptr<RestRequest> request)
    : GETv1versionReactor(connection, request)
{
}

VersionReactor::~VersionReactor()
{
}

bool VersionReactor::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    GETv1versionResponse response;
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "text/plain"));
    response.mBody = "v1";
    
    return response.Send(mConnection);
}
