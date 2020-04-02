#include "UpdateReactor.h"
#include "generated/POSTv1updateMessage.h"

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
    if (InputMessage() == nullptr)
    {
        LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
        return false;
    }

    RestResponse response;
    response.mHeaders.emplace_back(HttpHeader("Content-Type", "application/json"));

    return response.Send(mConnection, POSTv1updateMessage(InputMessage()));
}
