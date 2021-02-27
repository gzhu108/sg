#include "UpdateReactor.h"

using namespace microreactor;
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

    auto buffer = std::make_shared<std::string>();
    if (mRequestContent.Write(*buffer))
    {
        SetHttpBody(buffer, response);
    }

    return response.Send(mConnection);
}
