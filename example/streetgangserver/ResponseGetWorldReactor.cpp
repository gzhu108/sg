#include "ResponseGetWorldReactor.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


ResponseGetWorldReactor::ResponseGetWorldReactor(std::shared_ptr<Connection> connection, std::shared_ptr<ResponseGetWorld> message)
    : MessageReactor(connection, message)
{
}

ResponseGetWorldReactor::~ResponseGetWorldReactor()
{
}

bool ResponseGetWorldReactor::Process()
{
    auto latency = mRequesterMessage == nullptr ? 0 : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mRequesterMessage->GetRequestTime()).count();
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

    auto reactor = std::static_pointer_cast<RequestGetSceneReactor>(mRequesterMessage->ResponderReactor.get());
    SUBMIT(std::bind(&RequestGetSceneReactor::SendResponse, reactor, worldId, worldItems), reactor, this, "RequestGetSceneReactor::SendResponse");

    return true;
}
