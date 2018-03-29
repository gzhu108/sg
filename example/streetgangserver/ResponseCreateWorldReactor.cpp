#include "ResponseCreateWorldReactor.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


ResponseCreateWorldReactor::ResponseCreateWorldReactor(Connection& connection, std::shared_ptr<ResponseCreateWorld> message, std::shared_ptr<RequestCreateWorldReactor> reactor)
    : MessageReactor(connection, message)
    , mReactor(reactor)
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

    return SUBMIT(std::bind(&RequestCreateWorldReactor::SendResponse, mReactor, worldId, worldName), mReactor, this, "RequestCreateWorldReactor::SendResponse");
}
