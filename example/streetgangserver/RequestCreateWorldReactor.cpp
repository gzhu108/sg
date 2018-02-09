#include "RequestCreateWorldReactor.h"
#include "ResponseCreateWorld.h"
#include "StreetGangSessionManager.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestCreateWorldReactor::RequestCreateWorldReactor(Connection& connection, std::shared_ptr<RequestCreateWorld> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , mResponder(responder)
{
}

RequestCreateWorldReactor::~RequestCreateWorldReactor()
{
}

bool RequestCreateWorldReactor::Process()
{
    SessionId sessionId = StreetGangSessionManager::GetInstance().CreateWorld(InputMessage()->WorldName.cref());
    return mResponder->SendCreateWorldResponse(InputMessage()->TrackId.cref(), ResultCode::Success, sessionId, InputMessage()->WorldName.cref());
}
