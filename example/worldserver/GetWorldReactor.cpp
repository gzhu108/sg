#include "GetWorldReactor.h"
#include "WorldManager.h"
#include "ResponseGetWorld.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace worldserver;


GetWorldReactor::GetWorldReactor(Connection& connection, std::shared_ptr<RequestGetWorld> message, std::shared_ptr<WorldResponder> responder)
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

    std::vector<Point<float>> items;
    world->GetItems(items);

    return mResponder->SendGetWorldResponse(InputMessage()->TrackId.cref(), ResultCode::Success, world->Id.cref(), world->GetWorldBoundary(), items);
}
