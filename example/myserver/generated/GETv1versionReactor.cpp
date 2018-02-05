#include "GETv1versionReactor.h"

using namespace sg::microreactor;
using namespace myserver;


GETv1versionReactor::GETv1versionReactor(Connection& connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

GETv1versionReactor::~GETv1versionReactor()
{
}

bool GETv1versionReactor::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    // Send GETv1versionResponse or error response
    return RestResponse::SendErrorWith(mConnection, 501, "Not Implemented");
}
