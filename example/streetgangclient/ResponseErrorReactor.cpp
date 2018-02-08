#include "ResponseErrorReactor.h"
#include "TaskManagerSingleton.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


ResponseErrorReactor::ResponseErrorReactor(Connection& connection, std::shared_ptr<ResponseError> message, std::shared_ptr<StreetGangRequester> requester)
    : MessageReactor(connection, message)
    , mRequester(requester)
{
}

ResponseErrorReactor::~ResponseErrorReactor()
{
}

bool ResponseErrorReactor::Process()
{
    auto& errorCode = InputMessage()->Result.cref();
    auto& requestId = InputMessage()->RequestId.cref();

    LOG("ResponseErrorReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        mParentMessage == nullptr ? "0" : mParentMessage->TrackId->c_str(),
        requestId);

    STOP_TASK_MANAGER();

    return true;
}
