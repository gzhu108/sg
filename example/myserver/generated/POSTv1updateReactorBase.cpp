#include "POSTv1updateReactorBase.h"
#include "RestResponse.h"

using namespace sg::microreactor;
using namespace myserver;


POSTv1updateReactorBase::POSTv1updateReactorBase(Connection& connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

POSTv1updateReactorBase::~POSTv1updateReactorBase()
{
}

bool POSTv1updateReactorBase::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    // Send POSTv1updateResponse or error response
    return RestResponse::SendErrorWith(mConnection, 501, "POST [/v1/update] not Implemented");
}
