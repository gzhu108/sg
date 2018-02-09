#include "ResponseCreateWorldReactor.h"
#include <thread>
#include "Microreactor.h"
#include "ResponseCreateWorld.h"
#include "RequestGetScene.h"
#include "Rectangle.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


ResponseCreateWorldReactor::ResponseCreateWorldReactor(Connection& connection, std::shared_ptr<ResponseCreateWorld> message, std::shared_ptr<StreetGangRequester> requester)
    : MessageReactor(connection, message)
    , mRequester(requester)
{
}

ResponseCreateWorldReactor::~ResponseCreateWorldReactor()
{
}

bool ResponseCreateWorldReactor::Process()
{
    auto latency = mParentMessage == nullptr ? 0 : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mParentMessage->GetRequestTime()).count();
    auto& worldId = InputMessage()->WorldId.cref();
    auto& worldName = InputMessage()->WorldName.cref();

    LOG("[TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] ResponseCreateWorldReactor() [WorldId=" FMT_INT64 "] [WorldName=%s]",
        InputMessage()->TrackId->c_str(),
        latency,
        InputMessage()->Result.cref(),
        worldId,
        worldName.c_str());

    SendNextRequest();
    return true;
}

void ResponseCreateWorldReactor::SendNextRequest()
{
    // Get the scene from the world that just created
    streetgangapi::Rectangle<float> rect(0.4f, 0.4f, 0.2f, 0.2f);

    if (!mRequester->GetScene(InputMessage()->WorldId.cref(), rect))
    {
        LOG("Failed to get scenen");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        SUBMIT(std::bind(&ResponseCreateWorldReactor::SendNextRequest, this), shared_from_this(), this, "ResponseCreateWorldReactor::SendNextRequest");
    }
}
