#include "$method$funcReactorBase.h"

using namespace sg::microreactor;
using namespace $namespace;


$method$funcReactorBase::$method$funcReactorBase(Connection& connection, std::shared_ptr<RestRequest> request)
    : RestReactor(connection, request)
{
}

$method$funcReactorBase::~$method$funcReactorBase()
{
}

bool $method$funcReactorBase::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    // Send $method$funcResponse or error response
    return RestResponse::SendErrorWith(mConnection, 501, "$method [$path] not Implemented");
}
