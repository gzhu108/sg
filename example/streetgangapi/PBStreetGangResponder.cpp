#include "PBStreetGangResponder.h"
#include "BinarySerializer.h"
#include "PBResponseError.h"
#include "PBResponseGetVersion.h"
#include "PBResponseCreateWorld.h"
#include "PBResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBStreetGangResponder::PBStreetGangResponder(Connection& connection)
    : StreetGangResponder(connection)
{
}

PBStreetGangResponder::~PBStreetGangResponder()
{
}

bool PBStreetGangResponder::SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage)
{
    auto message = std::make_shared<PBResponseError>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->RequestId.set(requestId);
    message->ErrorMessage.set(errorMessage);
    return SendMessage(message);
}

bool PBStreetGangResponder::SendGetVersionResponse(const std::string& trackId, ResultCode result, const std::string& version)
{
    auto message = std::make_shared<PBResponseGetVersion>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->Version.set(version);
    return SendMessage(message);
}

bool PBStreetGangResponder::SendCreateWorldResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName)
{
    auto message = std::make_shared<PBResponseCreateWorld>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->WorldName.set(worldName);
    return SendMessage(message);
}

bool PBStreetGangResponder::SendGetSceneResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items)
{
    auto message = std::make_shared<PBResponseGetScene>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    message->Items.set(items);
    return SendMessage(message);
}

bool PBStreetGangResponder::SendMessage(std::shared_ptr<MessageBase> message)
{
    return StreetGangResponder::SendMessage(message);
}
