#include "RequestCreateWorldReactor.h"
#include "WorldRequester.h"
#include "WorldClient.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestCreateWorldReactor::RequestCreateWorldReactor(Connection& connection, std::shared_ptr<RequestCreateWorld> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , StreetGangReactor(responder)
{
}

RequestCreateWorldReactor::~RequestCreateWorldReactor()
{
}

bool RequestCreateWorldReactor::Process()
{
    auto requester = std::make_shared<worldapi::WorldRequester>(*WorldClient::GetInstance().GetConnection());
    return requester->CreateWorld(InputMessage()->WorldName.cref(), reinterpret_cast<uintptr_t>(this));
}

bool RequestCreateWorldReactor::SendResponse(const SessionId& sessionId, const std::string& worldName)
{
    return mResponder->SendCreateWorldResponse(InputMessage()->TrackId.cref(), ResultCode::Success, sessionId, worldName);
}
