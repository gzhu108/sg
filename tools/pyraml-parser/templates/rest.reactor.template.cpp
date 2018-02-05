#include "$method$funcReactor.h"

using namespace sg::microreactor;
using namespace $namespace;


$method$funcReactor::$method$funcReactor(Connection& connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

$method$funcReactor::~$method$funcReactor()
{
}

bool $method$funcReactor::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    // Send $method$funcResponse or error response
    return RestResponse::SendErrorWith(mConnection, 501, "Not Implemented");
}
