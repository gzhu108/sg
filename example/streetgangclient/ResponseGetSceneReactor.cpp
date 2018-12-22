#include "ResponseGetSceneReactor.h"
#include "ResponseGetVersionReactor.h"
#include <thread>
#include "Microreactor.h"
#include "ResponseGetScene.h"
#include "ResponseError.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


ResponseGetSceneReactor::ResponseGetSceneReactor(Connection& connection, std::shared_ptr<StreetGangRequester> requester)
    : Reactor(connection)
    , mRequester(requester)
{
}

ResponseGetSceneReactor::~ResponseGetSceneReactor()
{
}

bool ResponseGetSceneReactor::ProcessError(std::shared_ptr<Message> errorMessage)
{
    if (errorMessage == nullptr)
    {
        LOG("ResponseGetSceneReactor() ERROR: Invalid error message");
        return false;
    }

    auto responseMessage = std::static_pointer_cast<ResponseError>(errorMessage);
    auto& errorCode = responseMessage->Result.cref();
    auto& requestId = responseMessage->RequestId.cref();

    LOG("ResponseGetSceneReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        errorMessage->TrackId->c_str(),
        requestId);

    STOP_TASK_MANAGER();

    return true;
}

bool ResponseGetSceneReactor::ProcessTimeout(std::shared_ptr<Message> timedOutMessage)
{
    if (timedOutMessage == nullptr)
    {
        LOG("ResponseGetSceneReactor() ERROR: Invalid timeout message");
        return false;
    }

    auto message = std::static_pointer_cast<MessageBase>(timedOutMessage);
    auto& sessionId = message->MessageSessionId.cref();
    auto& requestId = message->Id.cref();

    LOG("ResponseGetSceneReactor() [sessionId=%d] [TrackId=%s] [RequestId=%d]",
        sessionId,
        message->TrackId->c_str(),
        requestId);

    return true;
}

bool ResponseGetSceneReactor::Process(std::shared_ptr<ResponseGetScene> response)
{
    if (response == nullptr)
    {
        return false;
    }

    auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - response->GetRequestTime()).count();
    auto& worldId = response->WorldId.cref();
    auto& rect = response->Rect.cref();
    auto& items = response->Items.cref();

    LOG("[TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] ResponseGetSceneReactor() [WorldId=" FMT_INT64 "] Rect(%f, %f, %f, %f), Items = %d",
        response->TrackId->c_str(),
        latency,
        response->Result.cref(),
        worldId,
        rect.mX, rect.mY, rect.mW, rect.mH,
        static_cast<const int32_t>(items.size()));

    SendNextRequest();
    return true;
}

void ResponseGetSceneReactor::SendNextRequest()
{
    if (!mRequester->GetVersion(std::make_shared<ResponseGetVersionReactor>(mConnection, mRequester)))
    {
        LOG("Failed to get the server version");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        SUBMIT_MEMBER(ResponseGetSceneReactor::SendNextRequest, "ResponseGetSceneReactor::SendNextRequest");
    }
}
