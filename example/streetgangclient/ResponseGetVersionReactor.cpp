#include "ResponseGetVersionReactor.h"
#include "ResponseCreateWorldReactor.h"
#include <thread>
#include "Microreactor.h"
#include "ResponseGetVersion.h"
#include "ResponseError.h"

using namespace microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


ResponseGetVersionReactor::ResponseGetVersionReactor(Connection& connection, std::shared_ptr<StreetGangRequester> requester)
    : Reactor(connection)
    , mRequester(requester)
{
}

ResponseGetVersionReactor::~ResponseGetVersionReactor()
{
}

bool ResponseGetVersionReactor::ProcessError(std::shared_ptr<Message> errorMessage)
{
    if (errorMessage == nullptr)
    {
        LOG("ResponseGetVersionReactor() ERROR: Invalid error message");
        return false;
    }

    auto responseMessage = std::static_pointer_cast<ResponseError>(errorMessage);
    auto& errorCode = responseMessage->Result.cref();
    auto& requestId = responseMessage->RequestId.cref();

    LOG("ResponseGetVersionReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        errorMessage->TrackId->c_str(),
        requestId);

    STOP_TASK_MANAGER();

    return true;
}

bool ResponseGetVersionReactor::ProcessTimeout(std::shared_ptr<Message> timedOutMessage)
{
    if (timedOutMessage == nullptr)
    {
        LOG("ResponseGetVersionReactor() ERROR: Invalid timeout message");
        return false;
    }

    auto message = std::static_pointer_cast<MessageBase>(timedOutMessage);
    auto& sessionId = message->MessageSessionId.cref();
    auto& requestId = message->Id.cref();

    LOG("ResponseGetVersionReactor() [sessionId=%d] [TrackId=%s] [RequestId=%d]",
        sessionId,
        message->TrackId->c_str(),
        requestId);

    return true;
}

bool ResponseGetVersionReactor::Process(std::shared_ptr<ResponseGetVersion> response)
{
    if (response == nullptr)
    {
        return false;
    }

    auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - response->GetRequestTime()).count();
    auto& version = response->Version.cref();

    LOG("[TrackId=%s] [Latency=" FMT_INT64 "] [Result=%d] ResponseGetVersionReactor() [version=%s]",
        response->TrackId->c_str(),
        latency,
        response->Result.cref(),
        version.c_str());

    SendNextRequest();
    return true;
}

void ResponseGetVersionReactor::SendNextRequest()
{
    static int worldNumber = 0;
    std::stringstream ss;
    ss << "World(" << worldNumber << ")";
    worldNumber++;

    if (!mRequester->CreateWorld(ss.str(), std::make_shared<ResponseCreateWorldReactor>(mConnection, mRequester)))
    {
        LOG("%s Failed to create a new world", ss.str().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //SUBMIT_MEMBER(ResponseGetVersionReactor::SendNextRequest, "ResponseGetVersionReactor::SendNextRequest");
    }
}
