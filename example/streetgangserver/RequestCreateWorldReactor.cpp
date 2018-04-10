#include "RequestCreateWorldReactor.h"
#include "Park.h"
#include "WorldRequester.h"
#include "WorldClient.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestCreateWorldReactor::RequestCreateWorldReactor(std::shared_ptr<Connection> connection, std::shared_ptr<RequestCreateWorld> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , StreetGangReactor(responder)
{
}

RequestCreateWorldReactor::~RequestCreateWorldReactor()
{
}

bool RequestCreateWorldReactor::Process()
{
    Park::ParkingLot().Add(reinterpret_cast<uintptr_t>(this), std::static_pointer_cast<Parkable>(shared_from_this()));

    worldapi::WorldRequester requester(WorldClient::GetInstance().GetConnection());
    return requester.CreateWorld(InputMessage()->WorldName.cref(), std::static_pointer_cast<Reactor>(shared_from_this()));
}

bool RequestCreateWorldReactor::SendResponse(const SessionId& sessionId, const std::string& worldName)
{
    std::vector<std::shared_ptr<Parkable>> parkables;
    Park::ParkingLot().Remove(reinterpret_cast<uintptr_t>(this), parkables);

    return mResponder->SendCreateWorldResponse(InputMessage()->TrackId.cref(), ResultCode::Success, sessionId, worldName);
}
