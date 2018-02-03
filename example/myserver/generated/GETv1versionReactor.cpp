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

    return RestResponse::SendResponse(mConnection, 501, "Not Implemented", "501 Not Implemented", true);
}
