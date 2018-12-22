#include "CreateWorldReactor.h"
#include "WorldManager.h"
#include "ResponseCreateWorld.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace worldserver;


CreateWorldReactor::CreateWorldReactor(Connection& connection, std::shared_ptr<RequestCreateWorld> message, std::shared_ptr<WorldResponder> responder)
    : MessageReactor(connection, message)
    , mResponder(responder)
{
}

CreateWorldReactor::~CreateWorldReactor()
{
}

bool CreateWorldReactor::Process()
{
    WorldId worldId = WorldManagerSingleton::GetInstance().CreateWorld(InputMessage()->WorldName.cref());
    return mResponder->SendCreateWorldResponse(InputMessage()->TrackId.cref(), ResultCode::Success, worldId, InputMessage()->WorldName.cref());
}
