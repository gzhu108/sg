#include "RequestGetSceneReactor.h"
#include "WorldServiceProvider.h"
#include "ResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestGetSceneReactor::RequestGetSceneReactor(Connection& connection, std::shared_ptr<RequestGetScene> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , mResponder(responder)
{
}

RequestGetSceneReactor::~RequestGetSceneReactor()
{
}

bool RequestGetSceneReactor::Process()
{
    if (mResponder == nullptr)
    {
        LOG("Invalid responder");
        return false;
    }

    if (mSession == nullptr)
    {
        return mResponder->SendErrorResponse(mConnection, InputMessage()->TrackId.cref(), ResultCode::ErrorBadRequest, InputMessage()->Id.cref(), "Scene not found");
    }

    auto session = std::static_pointer_cast<WorldServiceProvider>(mSession);

    std::vector<Point<float>> items;
    session->GetItemsInRect(InputMessage()->Rect.cref(), items);

    if (items.size() == 0)
    {
        LOG("No item found Rect(%f, %f, %f, %f)",
            InputMessage()->Rect->mX,
            InputMessage()->Rect->mY,
            InputMessage()->Rect->mW,
            InputMessage()->Rect->mH);
    }

    return mResponder->SendGetSceneResponse(mConnection, InputMessage()->TrackId.cref(), ResultCode::Success, mSession->Id.cref(), InputMessage()->Rect.cref(), items);
}
