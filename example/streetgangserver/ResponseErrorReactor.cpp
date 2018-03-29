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
    auto reactor = reinterpret_cast<sg::microreactor::Reactor*>(mParentMessage->UserData.get());
    auto streetGangReactor = dynamic_cast<StreetGangReactor*>(reactor);
    auto streetGangMessage = std::static_pointer_cast<streetgangapi::MessageBase>(reactor->Input());

    auto& errorCode = InputMessage()->Result.cref();
    auto& requestId = InputMessage()->RequestId.cref();

    LOG("ResponseErrorReactor() [Error=%d] [TrackId=%s] [RequestId=%d]",
        errorCode,
        mParentMessage == nullptr ? "0" : mParentMessage->TrackId->c_str(),
        requestId);

    return streetGangReactor->GetResponder()->SendErrorResponse(streetGangMessage->TrackId.cref(), (streetgangapi::ResultCode)errorCode, streetGangMessage->Id.cref(), InputMessage()->ErrorMessage.cref());

    return true;
}
