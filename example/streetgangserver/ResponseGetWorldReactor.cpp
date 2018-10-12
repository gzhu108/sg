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
    auto& worldId = InputMessage()->WorldId.cref();
    auto& worldRect = InputMessage()->Rect.cref();
    auto& worldItems = InputMessage()->Items.cref();

    if (mOriginalMessage == nullptr)
    {
        LOG("ResponseGetWorldReactor() ERROR: Invalid original message [TrackId=%s] [Result=%d] [WorldId=" FMT_INT64 "] Rect(%f, %f, %f, %f), Items = %d",
            InputMessage()->TrackId->c_str(),
            InputMessage()->Result.cref(),
            worldId,
            worldRect.mX, worldRect.mY, worldRect.mW, worldRect.mH,
            static_cast<const int32_t>(worldItems.size()));

        return false;
    }

    auto latency = mOriginalMessage == nullptr ? 0 : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mOriginalMessage->GetRequestTime()).count();

    LOG("ResponseGetWorldReactor() INFO: [TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] [WorldId=" FMT_INT64 "] Rect(%f, %f, %f, %f), Items = %d",
        InputMessage()->TrackId->c_str(),
        latency,
        InputMessage()->Result.cref(),
        worldId,
        worldRect.mX, worldRect.mY, worldRect.mW, worldRect.mH,
        static_cast<const int32_t>(worldItems.size()));

    auto reactor = std::static_pointer_cast<RequestGetSceneReactor>(mOriginalMessage->OriginalReactor.get());
    SUBMIT(std::bind(&RequestGetSceneReactor::SendResponse, reactor, worldId, worldItems), reactor, this, typeid(*reactor).hash_code(), "RequestGetSceneReactor::SendResponse");

    return true;
}
