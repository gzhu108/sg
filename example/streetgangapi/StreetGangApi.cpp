#include "StreetGangApi.h"
#include "Uuid.h"
#include "StreetGangIds.h"

#include "RequestByebye.h"
#include "RequestGetVersion.h"
#include "RequestCreateWorld.h"
#include "RequestGetScene.h"
#include "ResponseError.h"

using namespace sg::microreactor;
using namespace streetgangapi;


StreetGangApi::StreetGangApi(Connection& connection, std::shared_ptr<StreetGangRequestEncoder> messageEncoder)
    : Api(connection, messageEncoder)
{
}

StreetGangApi::~StreetGangApi()
{
}

bool StreetGangApi::Byebye()
{
    auto request = std::make_shared<RequestByebye>();
    request->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(request);
}

bool StreetGangApi::GetVersion()
{
    auto request = std::make_shared<RequestGetVersion>();
    request->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(request);
}

bool StreetGangApi::CreateWorld(const std::string& worldName)
{
    auto request = std::make_shared<RequestCreateWorld>();
    request->TrackId.set(Uuid::GenerateUuid().ToString());
    request->WorldName.set(worldName);
    return SendMessage(request);
}

bool StreetGangApi::GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect)
{
    auto request = std::make_shared<RequestGetScene>();
    request->TrackId.set(Uuid::GenerateUuid().ToString());
    request->WorldId.set(worldId);
    request->Rect.set(rect);
    return SendMessage(request);
}
