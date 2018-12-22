#include "PBRequestByebye.h"
#include "PBRequestGetVersion.h"
#include "PBRequestCreateWorld.h"
#include "PBRequestGetScene.h"
#include "PBResponseError.h"
#include "PBStreetGangRequester.h"
#include "BinarySerializer.h"
#include "Uuid.h"

using namespace sg::microreactor;
using namespace streetgangapi;


PBStreetGangRequester::PBStreetGangRequester(Connection& connection)
    : StreetGangRequester(connection)
{
}

PBStreetGangRequester::~PBStreetGangRequester()
{
}

bool PBStreetGangRequester::Byebye(std::shared_ptr<Reactor> client)
{
    auto message = std::make_shared<PBRequestByebye>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(message, client);
}

bool PBStreetGangRequester::GetVersion(std::shared_ptr<Reactor> client)
{
    auto message = std::make_shared<PBRequestGetVersion>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    return SendMessage(message, client);
}

bool PBStreetGangRequester::CreateWorld(const std::string& worldName, std::shared_ptr<Reactor> client)
{
    auto message = std::make_shared<PBRequestCreateWorld>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldName.set(worldName);
    return SendMessage(message, client);
}

bool PBStreetGangRequester::GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect, std::shared_ptr<Reactor> client)
{
    auto message = std::make_shared<PBRequestGetScene>();
    message->TrackId.set(Uuid::GenerateUuid().ToString());
    message->WorldId.set(worldId);
    message->Rect.set(rect);
    return SendMessage(message, client);
}

bool PBStreetGangRequester::SendMessage(std::shared_ptr<MessageBase> message, std::shared_ptr<Reactor> client)
{
    return StreetGangRequester::SendMessage(message, client);
}
