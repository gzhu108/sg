#include "GetWorldReactor.h"
#include "WorldManager.h"
#include "ResponseGetWorld.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace worldserver;


GetWorldReactor::GetWorldReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RequestGetWorld> message, std::shared_ptr<WorldResponder> responder)
    : MessageReactor(connection, message)
    , mResponder(responder)
{
}

GetWorldReactor::~GetWorldReactor()
{
}

bool GetWorldReactor::Process()
{
    if (mResponder == nullptr)
    {
        LOG("Invalid responder");
        return false;
    }

    auto world = WorldManagerSingleton::GetInstance().GetWorld(InputMessage()->WorldId.cref());
    if (world == nullptr)
    {
        return mResponder->SendErrorResponse(InputMessage()->TrackId.cref(), ResultCode::ErrorBadRequest, InputMessage()->Id.cref(), "World not found");
    }

    return mResponder->SendGetWorldResponse(InputMessage()->TrackId.cref(), ResultCode::Success, *world);
}
