#include "ResponseErrorReactor.h"
#include "TaskManagerSingleton.h"
#include "MessageBase.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


ResponseErrorReactor::ResponseErrorReactor(Connection& connection, std::shared_ptr<ResponseError> message)
    : MessageReactor(connection, message)
{
}

ResponseErrorReactor::~ResponseErrorReactor()
{
}

bool ResponseErrorReactor::Process()
{
    auto reactor = mRequesterMessage->ResponderReactor.get();
    auto streetGangReactor = std::dynamic_pointer_cast<StreetGangReactor>(reactor);
    auto streetGangMessage = std::static_pointer_cast<streetgangapi::MessageBase>(reactor->Input());

    auto& errorCode = InputMessage()->Result.cref();
    auto& requestId = InputMessage()->RequestId.cref();

    LOG("ResponseErrorReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        mRequesterMessage == nullptr ? "0" : mRequesterMessage->TrackId->c_str(),
        requestId);

    return streetGangReactor->GetResponder()->SendErrorResponse(streetGangMessage->TrackId.cref(), (streetgangapi::ResultCode)errorCode, streetGangMessage->Id.cref(), InputMessage()->ErrorMessage.cref());
}
