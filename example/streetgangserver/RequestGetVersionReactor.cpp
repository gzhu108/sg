#include "RequestGetVersionReactor.h"
#include "ResponseGetVersion.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestGetVersionReactor::RequestGetVersionReactor(Connection& connection, std::shared_ptr<RequestGetVersion> message)
    : MessageReactor(connection, message)
{
}

RequestGetVersionReactor::~RequestGetVersionReactor()
{
}

bool RequestGetVersionReactor::Process()
{
    if (mConnection.GetProfile() == nullptr || mConnection.GetProfile()->Configuration.cref() == nullptr)
    {
        return false;
    }

    std::string version;
    mConnection.GetProfile()->Configuration.cref()->GetValue("Version", version);

    auto response = std::make_shared<ResponseGetVersion>();
    response->TrackId.set(InputMessage()->TrackId.cref());
    response->Result.set(static_cast<int32_t>(ResultCode::Success));
    response->Version.set(version);

    return SendMessage(response);
    //return SubmitTask(std::bind(&RequestGetVersionReactor::SendMessage, this, response), "RequestGetVersionReactor::SendMessage");
}
