#include "ResponseGetSceneReactor.h"
#include <thread>
#include "Microreactor.h"
#include "ResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


ResponseGetSceneReactor::ResponseGetSceneReactor(std::shared_ptr<Connection> connection, std::shared_ptr<ResponseGetScene> message, std::shared_ptr<StreetGangRequester> requester)
    : MessageReactor(connection, message)
    , mRequester(requester)
{
}

ResponseGetSceneReactor::~ResponseGetSceneReactor()
{
}

bool ResponseGetSceneReactor::Process()
{
    auto latency = mOriginalMessage == nullptr ? 0 : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mOriginalMessage->GetRequestTime()).count();
    auto& worldId = InputMessage()->WorldId.cref();
    auto& rect = InputMessage()->Rect.cref();
    auto& items = InputMessage()->Items.cref();

    LOG("[TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] ResponseGetSceneReactor() [WorldId=" FMT_INT64 "] Rect(%f, %f, %f, %f), Items = %d",
        InputMessage()->TrackId->c_str(),
        latency,
        InputMessage()->Result.cref(),
        worldId,
        rect.mX, rect.mY, rect.mW, rect.mH,
        static_cast<const int32_t>(items.size()));

    SendNextRequest();
    return true;
}

void ResponseGetSceneReactor::SendNextRequest()
{
    if (!mRequester->GetVersion())
    {
        LOG("Failed to get the server version");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        SUBMIT(std::bind(&ResponseGetSceneReactor::SendNextRequest, this), shared_from_this(), this, "ResponseGetSceneReactor::SendNextRequest");
    }
}
