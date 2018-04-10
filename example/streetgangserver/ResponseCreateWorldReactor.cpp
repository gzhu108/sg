#include "ResponseCreateWorldReactor.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


ResponseCreateWorldReactor::ResponseCreateWorldReactor(std::shared_ptr<Connection> connection, std::shared_ptr<ResponseCreateWorld> message)
    : MessageReactor(connection, message)
{
}

ResponseCreateWorldReactor::~ResponseCreateWorldReactor()
{
}

bool ResponseCreateWorldReactor::Process()
{
    auto latency = mRequesterMessage == nullptr ? 0 : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mRequesterMessage->GetRequestTime()).count();
    auto& worldId = InputMessage()->WorldId.cref();
    auto& worldName = InputMessage()->WorldName.cref();

    LOG("[TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] ResponseCreateWorldReactor() [WorldId=" FMT_INT64 "] [WorldName=%s]",
        InputMessage()->TrackId->c_str(),
        latency,
        InputMessage()->Result.cref(),
        worldId,
        worldName.c_str());

    auto reactor = std::static_pointer_cast<RequestCreateWorldReactor>(mRequesterMessage->ResponderReactor.get());
    SUBMIT(std::bind(&RequestCreateWorldReactor::SendResponse, reactor, worldId, worldName), reactor, this, "RequestCreateWorldReactor::SendResponse");

    return true;
}
