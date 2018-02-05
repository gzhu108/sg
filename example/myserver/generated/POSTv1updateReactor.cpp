#include "POSTv1updateReactor.h"

using namespace sg::microreactor;
using namespace myserver;


POSTv1updateReactor::POSTv1updateReactor(Connection& connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

POSTv1updateReactor::~POSTv1updateReactor()
{
}

bool POSTv1updateReactor::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    // Send POSTv1updateResponse or error response
    return RestResponse::SendErrorWith(mConnection, 501, "Not Implemented");
}
