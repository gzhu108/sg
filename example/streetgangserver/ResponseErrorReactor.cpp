#include "ResponseErrorReactor.h"
#include "TaskManagerSingleton.h"
#include "MessageBase.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


ResponseErrorReactor::ResponseErrorReactor(std::shared_ptr<Connection> connection, std::shared_ptr<ResponseError> message)
    : MessageReactor(connection, message)
{
}

ResponseErrorReactor::~ResponseErrorReactor()
{
}

bool ResponseErrorReactor::Process()
{
    if (mOriginalMessage == nullptr)
    {
        LOG("ResponseErrorReactor() ERROR: Invalid original message");
        return false;
    }

    auto originalReactor = mOriginalMessage->OriginalReactor.get();
    auto streetGangReactor = std::dynamic_pointer_cast<StreetGangReactor>(originalReactor);
    auto streetGangMessage = std::static_pointer_cast<streetgangapi::MessageBase>(originalReactor->Input());

    auto& errorCode = InputMessage()->Result.cref();
    auto& requestId = InputMessage()->RequestId.cref();

    LOG("ResponseErrorReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        mOriginalMessage == nullptr ? "0" : mOriginalMessage->TrackId->c_str(),
        requestId);

    return streetGangReactor->GetResponder()->SendErrorResponse(streetGangMessage->TrackId.cref(), (streetgangapi::ResultCode)errorCode, streetGangMessage->Id.cref(), InputMessage()->ErrorMessage.cref());
}
