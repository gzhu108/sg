#include "ResponseErrorReactor.h"
#include "TaskManagerSingleton.h"
#include "MessageBase.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


ResponseErrorReactor::ResponseErrorReactor(Connection& connection, std::shared_ptr<ResponseError> message, std::shared_ptr<StreetGangReactor> reactor)
    : MessageReactor(connection, message)
    , mReactor(reactor)
{
}

ResponseErrorReactor::~ResponseErrorReactor()
{
}

bool ResponseErrorReactor::Process()
{
    auto reactor = std::dynamic_pointer_cast<sg::microreactor::Reactor>(mReactor);
    auto streetGangMessage = std::static_pointer_cast<streetgangapi::MessageBase>(reactor->Input());

    auto& errorCode = InputMessage()->Result.cref();
    auto& requestId = InputMessage()->RequestId.cref();

    LOG("ResponseErrorReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        mParentMessage == nullptr ? "0" : mParentMessage->TrackId->c_str(),
        requestId);

    return mReactor->GetResponder()->SendErrorResponse(streetGangMessage->TrackId.cref(), (streetgangapi::ResultCode)errorCode, streetGangMessage->Id.cref(), InputMessage()->ErrorMessage.cref());

    return true;
}
