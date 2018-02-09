#include "PBStreetGangRequester.h"
#include "BinarySerializer.h"
#include "Uuid.h"
#include "StreetGangIds.h"

#include "PBRequestByebye.h"
#include "PBRequestGetVersion.h"
#include "PBRequestCreateWorld.h"
#include "PBRequestGetScene.h"
#include "PBResponseError.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBStreetGangRequester::PBStreetGangRequester(Connection& connection)
    : StreetGangRequester(connection)
{
}

PBStreetGangRequester::~PBStreetGangRequester()
{
}

bool PBStreetGangRequester::Byebye()
{
    auto message = std::make_shared<PBRequestByebye>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(message);
}

bool PBStreetGangRequester::GetVersion()
{
    auto message = std::make_shared<PBRequestGetVersion>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(message);
}

bool PBStreetGangRequester::CreateWorld(const std::string& worldName)
{
    auto message = std::make_shared<RequestCreateWorld>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldName.set(worldName);
    return SendMessage(message);
}

bool PBStreetGangRequester::GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect)
{
    auto message = std::make_shared<RequestGetScene>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    return SendMessage(message);
}

bool PBStreetGangRequester::SendMessage(std::shared_ptr<MessageBase> message)
{
    return StreetGangRequester::SendMessage(message);
}
