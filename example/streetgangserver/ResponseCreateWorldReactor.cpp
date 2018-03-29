#include "ResponseCreateWorldReactor.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


ResponseCreateWorldReactor::ResponseCreateWorldReactor(Connection& connection, std::shared_ptr<ResponseCreateWorld> message)
    : MessageReactor(connection, message)
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

    auto reactor = reinterpret_cast<RequestCreateWorldReactor*>(mParentMessage->UserData.get());
    return SUBMIT(std::bind(&RequestCreateWorldReactor::SendResponse, reactor, worldId, worldName), reactor->shared_from_this(), this, "RequestCreateWorldReactor::SendResponse");
}
