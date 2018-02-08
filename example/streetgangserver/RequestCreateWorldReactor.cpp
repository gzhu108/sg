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

    auto response = std::make_shared<ResponseCreateWorld>();
    response->TrackId.set(InputMessage()->TrackId.cref());
    response->Result.set(static_cast<int32_t>(ResultCode::Success));
    response->WorldName.set(InputMessage()->WorldName.cref());
    response->WorldId.set(sessionId);

    return SendMessage(response);
    //return SubmitTask(std::bind(&RequestCreateWorldReactor::SendMessage, this, response), "RequestCreateWorldReactor::SendMessage");
}
