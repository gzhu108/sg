#include "GETv1versionReactorBase.h"

using namespace sg::microreactor;
using namespace myserver;


GETv1versionReactorBase::GETv1versionReactorBase(Connection& connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

GETv1versionReactorBase::~GETv1versionReactorBase()
{
}

bool GETv1versionReactorBase::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    // Send GETv1versionResponse or error response
    return RestResponse::SendErrorWith(mConnection, 501, "GET [/v1/version] not Implemented");
}
