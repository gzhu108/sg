#include "TaskManagerSingleton.h"
#include "RequestCreateWorldReactor.h"
#include "WorldRequester.h"
#include "WorldClient.h"

using namespace sg::microreactor;
using namespace worldapi;
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
    auto response = requester.CreateWorld(InputMessage()->WorldName.cref(), std::static_pointer_cast<Reactor>(shared_from_this()));
    if (response != nullptr)
    {
        // Found the world in the cache
        return ProcessCreateWorldResponse(response);
    }

    // Request has been sent
    return true;
}

bool RequestCreateWorldReactor::ProcessError(std::shared_ptr<Message> errorMessage)
{
    if (errorMessage == nullptr)
    {
        LOG("RequestCreateWorldReactor() ERROR: Invalid error message");
        return false;
    }

    auto responseMessage = std::static_pointer_cast<worldapi::ResponseError>(errorMessage);
    auto& errorCode = responseMessage->Result.cref();
    auto& requestId = responseMessage->RequestId.cref();

    LOG("ResponseErrorReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        InputMessage()->TrackId->c_str(),
        requestId);

    return mResponder->SendErrorResponse(InputMessage()->TrackId.cref(), (streetgangapi::ResultCode)errorCode, InputMessage()->Id.cref(), responseMessage->ErrorMessage.cref());
}

bool RequestCreateWorldReactor::ProcessTimeout(std::shared_ptr<Message> timedOutMessage)
{
    return mResponder->SendErrorResponse(InputMessage()->TrackId.cref(), streetgangapi::ResultCode::ErrorTimeout, InputMessage()->Id.cref(), "StreetGangServer timeout");
}

bool RequestCreateWorldReactor::ProcessCreateWorldResponse(std::shared_ptr<ResponseCreateWorld> response)
{
    if (response == nullptr)
    {
        LOG("ProcessCreateWorldResponse() ERROR: Invalid response");
        return false;
    }

    auto& worldId = response->WorldId.cref();
    auto& worldName = response->WorldName.cref();
    auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - response->GetRequestTime()).count();

    LOG("ProcessCreateWorldResponse() INOFO: [TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] [WorldId=" FMT_INT64 "] [WorldName=%s]",
        response->TrackId->c_str(),
        latency,
        response->Result.cref(),
        worldId,
        worldName.c_str());

    return SendResponse(worldId, worldName);
}

bool RequestCreateWorldReactor::SendResponse(const SessionId& sessionId, const std::string& worldName)
{
    return mResponder->SendCreateWorldResponse(InputMessage()->TrackId.cref(), streetgangapi::ResultCode::Success, sessionId, worldName);
}
