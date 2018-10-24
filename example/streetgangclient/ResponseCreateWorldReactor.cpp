#include "ResponseCreateWorldReactor.h"
#include "ResponseGetSceneReactor.h"
#include <thread>
#include "Microreactor.h"
#include "ResponseCreateWorld.h"
#include "RequestGetScene.h"
#include "Rectangle.h"
#include "ResponseError.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


ResponseCreateWorldReactor::ResponseCreateWorldReactor(std::shared_ptr<Connection> connection, std::shared_ptr<StreetGangRequester> requester)
    : Reactor(connection)
    , mRequester(requester)
{
}

ResponseCreateWorldReactor::~ResponseCreateWorldReactor()
{
}

bool ResponseCreateWorldReactor::ProcessError(std::shared_ptr<Message> errorMessage)
{
    if (errorMessage == nullptr)
    {
        LOG("ResponseCreateWorldReactor() ERROR: Invalid error message");
        return false;
    }

    auto responseMessage = std::static_pointer_cast<ResponseError>(errorMessage);
    auto& errorCode = responseMessage->Result.cref();
    auto& requestId = responseMessage->RequestId.cref();

    LOG("ResponseCreateWorldReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        errorMessage->TrackId->c_str(),
        requestId);

    STOP_TASK_MANAGER();

    return true;
}

bool ResponseCreateWorldReactor::ProcessTimeout(std::shared_ptr<Message> timedOutMessage)
{
    if (timedOutMessage == nullptr)
    {
        LOG("ResponseCreateWorldReactor() ERROR: Invalid timeout message");
        return false;
    }

    auto message = std::static_pointer_cast<MessageBase>(timedOutMessage);
    auto& sessionId = message->MessageSessionId.cref();
    auto& requestId = message->Id.cref();

    LOG("ResponseCreateWorldReactor() [sessionId=%d] [TrackId=%s] [RequestId=%d]",
        sessionId,
        message->TrackId->c_str(),
        requestId);

    return true;
}

bool ResponseCreateWorldReactor::Process(std::shared_ptr<ResponseCreateWorld> response)
{
    if (response == nullptr)
    {
        return false;
    }

    auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - response->GetRequestTime()).count();
    auto& worldId = response->WorldId.cref();
    auto& worldName = response->WorldName.cref();

    LOG("[TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] ResponseCreateWorldReactor() [WorldId=" FMT_INT64 "] [WorldName=%s]",
        response->TrackId->c_str(),
        latency,
        response->Result.cref(),
        worldId,
        worldName.c_str());

    SendNextRequest(response->WorldId.cref());
    return true;
}

void ResponseCreateWorldReactor::SendNextRequest(uintptr_t worldId)
{
    // Get the scene from the world that just created
    streetgangapi::Rectangle<float> rect(0.4f, 0.4f, 0.2f, 0.2f);

    if (!mRequester->GetScene(worldId, rect, std::make_shared<ResponseGetSceneReactor>(mConnection, mRequester)))
    {
        LOG("Failed to get scenen");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        SUBMIT(std::bind(&ResponseCreateWorldReactor::SendNextRequest, this, worldId), this, this, typeid(*this).hash_code(), "ResponseCreateWorldReactor::SendNextRequest");
    }
}
