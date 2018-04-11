#include "VersionReactor.h"
#include "GETv1versionResponse.h"

using namespace sg::microreactor;
using namespace myserver;


VersionReactor::VersionReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RestRequest> request)
    : GETv1versionReactorBase(connection, request)
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
    response.Version.set("v1");
    return response.Send(*mConnection);
}
