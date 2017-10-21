#include "RequestGetSceneReactor.h"
#include "WorldServiceProvider.h"
#include "ResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestGetSceneReactor::RequestGetSceneReactor(Connection& connection, std::shared_ptr<RequestGetScene> message)
    : MessageReactor(connection, message)
{
}

RequestGetSceneReactor::~RequestGetSceneReactor()
{
}

bool RequestGetSceneReactor::Process()
{
    if (mSession == nullptr)
    {
        auto errorResponse = std::make_shared<ResponseError>();
        errorResponse->Result.set((int32_t)ResultCode::ErrorBadRequest);
        errorResponse->RequestId.set(InputMessage()->Id.cref());
        errorResponse->ErrorMessage.set("Scene not found");
        return SendMessage(errorResponse);
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

    auto response = std::make_shared<ResponseGetScene>();
    response->TrackId.set(InputMessage()->TrackId.cref());
    response->Result.set(static_cast<int32_t>(ResultCode::Success));
    response->WorldId.set(mSession->Id.cref());
    response->Rect.set(InputMessage()->Rect.cref());
    response->Items.set(items);

    return SendMessage(response);
    //return SubmitTask(std::bind(&RequestGetSceneReactor::SendMessage, this, response), "RequestGetVersionReactor::SendMessage");
}
