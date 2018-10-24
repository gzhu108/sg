#include "TaskManagerSingleton.h"
#include "RequestGetSceneReactor.h"
#include "WorldRequester.h"
#include "WorldClient.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestGetSceneReactor::RequestGetSceneReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RequestGetScene> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , StreetGangReactor(responder)
{
}

RequestGetSceneReactor::~RequestGetSceneReactor()
{
}

bool RequestGetSceneReactor::Process()
{
    worldapi::WorldRequester requester(WorldClient::GetInstance().GetConnection(), WorldClient::GetInstance().GetWorldCache());
    auto response = requester.GetWorld(InputMessage()->WorldId.cref(), std::static_pointer_cast<Reactor>(shared_from_this()));
    if (response != nullptr)
    {
        return ProcessGetWorldResponse(response);
    }

    return true;
}

bool RequestGetSceneReactor::ProcessError(std::shared_ptr<Message> errorMessage)
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

bool RequestGetSceneReactor::ProcessTimeout(std::shared_ptr<Message> timedOutMessage)
{
    return mResponder->SendErrorResponse(InputMessage()->TrackId.cref(), streetgangapi::ResultCode::ErrorTimeout, InputMessage()->Id.cref(), "StreetGangServer timeout");
}

bool RequestGetSceneReactor::ProcessGetWorldResponse(std::shared_ptr<worldapi::ResponseGetWorld> response)
{
    if (response == nullptr)
    {
        LOG("ProcessGetWorldResponse() ERROR: Invalid response");
        return false;
    }

    auto& worldId = response->World->mId;
    auto& worldRect = response->World->mWorldBoundary;
    auto& worldItems = response->World->mItems;
    auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - response->GetRequestTime()).count();

    LOG("ResponseGetWorldReactor() INFO: [TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] [WorldId=" FMT_INT64 "] Rect(%f, %f, %f, %f), Items = %d",
        response->TrackId->c_str(),
        latency,
        response->Result.cref(),
        worldId,
        worldRect.mX, worldRect.mY, worldRect.mW, worldRect.mH,
        static_cast<const int32_t>(worldItems.size()));

    return SendResponse(worldId, worldItems);
}

bool RequestGetSceneReactor::SendResponse(const streetgangapi::SessionId& sessionId, const std::vector<worldapi::Point<float>>& items)
{
    if (mResponder == nullptr)
    {
        LOG("Invalid responder");
        return false;
    }

    std::vector<Point<float>> targetItems;
    if (GetItemsInRect(InputMessage()->Rect.cref(), items, targetItems) == 0)
    {
        LOG("No item found Rect(%f, %f, %f, %f)",
            InputMessage()->Rect->mX,
            InputMessage()->Rect->mY,
            InputMessage()->Rect->mW,
            InputMessage()->Rect->mH);
    }

    return mResponder->SendGetSceneResponse(InputMessage()->TrackId.cref(), ResultCode::Success, sessionId, InputMessage()->Rect.cref(), targetItems);
}

uint64_t RequestGetSceneReactor::GetItemsInRect(const streetgangapi::Rectangle<float>& rect, const std::vector<worldapi::Point<float>>& sourceItems, std::vector<streetgangapi::Point<float>>& targetItems)
{
    targetItems.clear();

    for (auto& item : sourceItems)
    {
        if (item.mX >= rect.mX &&
            item.mX < (rect.mX + rect.mW) &&
            item.mY >= rect.mY &&
            item.mY < (rect.mY + rect.mH))
        {
            targetItems.emplace_back(streetgangapi::Point<float>(item.mX, item.mY, item.mZ));
        }
    }

    return targetItems.size();
}
