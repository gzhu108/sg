#include "TaskManagerSingleton.h"
#include "RequestCreateWorldReactor.h"
#include "ResponseCreateWorldReactor.h"
#include "WorldRequester.h"
#include "WorldClient.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestCreateWorldReactor::RequestCreateWorldReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RequestCreateWorld> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , StreetGangReactor(responder)
{
}

RequestCreateWorldReactor::~RequestCreateWorldReactor()
{
}

bool RequestCreateWorldReactor::Process()
{
    worldapi::WorldRequester requester(WorldClient::GetInstance().GetConnection(), WorldClient::GetInstance().GetWorldCache());
    auto message = requester.CreateWorld(InputMessage()->WorldName.cref(), std::static_pointer_cast<Reactor>(shared_from_this()));
    if (message != nullptr)
    {
        return SendResponse(message->WorldId.cref(), message->WorldName.cref());
    }

    return true;
}

bool RequestCreateWorldReactor::SendResponse(const SessionId& sessionId, const std::string& worldName)
{
    return mResponder->SendCreateWorldResponse(InputMessage()->TrackId.cref(), ResultCode::Success, sessionId, worldName);
}
