#include "RequestGetVersionReactor.h"
#include "ResponseGetVersion.h"
#include "ConfigurationSingleton.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestGetVersionReactor::RequestGetVersionReactor(Connection& connection, std::shared_ptr<RequestGetVersion> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , StreetGangReactor(responder)
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

    if (ConfigurationSingleton::GetConfiguration() == nullptr)
    {
        return false;
    }

    std::string version;
    ConfigurationSingleton::GetConfiguration()->GetValue("Version", version);

    return mResponder->SendGetVersionResponse(InputMessage()->TrackId.cref(), ResultCode::Success, version);
}
