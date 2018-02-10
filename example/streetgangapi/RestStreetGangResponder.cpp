#include "RestStreetGangResponder.h"
#include "RestResponseError.h"
#include "RestResponseGetVersion.h"
#include "RestResponseCreateWorld.h"
#include "RestResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;


RestStreetGangResponder::RestStreetGangResponder(Connection& connection)
    : StreetGangResponder(connection)
{
}

RestStreetGangResponder::~RestStreetGangResponder()
{
}

bool RestStreetGangResponder::SendErrorResponse(const std::string& trackId, ResultCode result, const int32_t& requestId, const std::string& errorMessage)
{
    auto message = std::make_shared<RestResponseError>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->RequestId.set(requestId);
    message->ErrorMessage.set(errorMessage);
    return SendMessage(message);
}

bool RestStreetGangResponder::SendGetVersionResponse(const std::string& trackId, ResultCode result, const std::string& version)
{
    auto message = std::make_shared<RestResponseGetVersion>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->Version.set(version);
    return SendMessage(message);
}

bool RestStreetGangResponder::SendCreateWorldResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const std::string& worldName)
{
    auto message = std::make_shared<RestResponseCreateWorld>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->WorldName.set(worldName);
    return SendMessage(message);
}

bool RestStreetGangResponder::SendGetSceneResponse(const std::string& trackId, ResultCode result, const streetgangapi::SessionId& worldId, const streetgangapi::Rectangle<float>& rect, const std::vector<streetgangapi::Point<float>>& items)
{
    auto message = std::make_shared<RestResponseGetScene>();
    message->TrackId.set(trackId);
    message->Result.set(static_cast<int32_t>(result));
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    message->Items.set(items);
    return SendMessage(message);
}

bool RestStreetGangResponder::SendMessage(std::shared_ptr<MessageBase> message)
{
    return StreetGangResponder::SendMessage(message);
}
