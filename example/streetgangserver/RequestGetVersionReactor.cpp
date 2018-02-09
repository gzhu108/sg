#include "RequestGetVersionReactor.h"
#include "ResponseGetVersion.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestGetVersionReactor::RequestGetVersionReactor(Connection& connection, std::shared_ptr<RequestGetVersion> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , mResponder(responder)
{
}

RequestGetVersionReactor::~RequestGetVersionReactor()
{
}

bool RequestGetVersionReactor::Process()
{
    if (mResponder == nullptr)
    {
        LOG("Invalid responder");
        return false;
    }

    if (mConnection.GetProfile() == nullptr || mConnection.GetProfile()->Configuration.cref() == nullptr)
    {
        return false;
    }

    std::string version;
    mConnection.GetProfile()->Configuration.cref()->GetValue("Version", version);

    return mResponder->SendGetVersionResponse(InputMessage()->TrackId.cref(), ResultCode::Success, version);
}
