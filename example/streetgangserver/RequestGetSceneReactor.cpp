#include "TaskManagerSingleton.h"
#include "RequestGetSceneReactor.h"
#include "ResponseGetWorldReactor.h"
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
    auto message = requester.GetWorld(InputMessage()->WorldId.cref(), std::static_pointer_cast<Reactor>(shared_from_this()));
    if (message != nullptr)
    {
        return SendResponse(InputMessage()->WorldId.cref(), message->World->mItems);
    }

    return true;
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
