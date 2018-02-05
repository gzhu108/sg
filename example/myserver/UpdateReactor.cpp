#include "UpdateReactor.h"
#include "POSTv1updateResponse.h"

using namespace sg::microreactor;
using namespace myserver;


UpdateReactor::UpdateReactor(Connection& connection, std::shared_ptr<RestRequest> request)
    : POSTv1updateReactorBase(connection, request)
{
}

UpdateReactor::~UpdateReactor()
{
}

bool UpdateReactor::Process()
{
    if (Request() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    POSTv1updateResponse response;
    return response.Send(mConnection);
}
