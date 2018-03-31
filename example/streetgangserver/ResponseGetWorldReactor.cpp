#include "ResponseGetWorldReactor.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


ResponseGetWorldReactor::ResponseGetWorldReactor(Connection& connection, std::shared_ptr<ResponseGetWorld> message)
    : MessageReactor(connection, message)
{
}

ResponseGetWorldReactor::~ResponseGetWorldReactor()
{
}

bool ResponseGetWorldReactor::Process()
{
    auto latency = mParentMessage == nullptr ? 0 : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mParentMessage->GetRequestTime()).count();
    auto& worldId = InputMessage()->WorldId.cref();
    auto& worldRect = InputMessage()->Rect.cref();
    auto& worldItems = InputMessage()->Items.cref();

    LOG("[TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] ResponseGetWorldReactor() [WorldId=" FMT_INT64 "] Rect(%f, %f, %f, %f), Items = %d",
        InputMessage()->TrackId->c_str(),
        latency,
        InputMessage()->Result.cref(),
        worldId,
        worldRect.mX, worldRect.mY, worldRect.mW, worldRect.mH,
        static_cast<const int32_t>(worldItems.size()));

    auto reactor = reinterpret_cast<RequestGetSceneReactor*>(mParentMessage->UserData.get());
    SUBMIT(std::bind(&RequestGetSceneReactor::SendResponse, reactor, worldId, worldItems), reactor->shared_from_this(), this, "RequestGetSceneReactor::SendResponse");

    return true;
}
