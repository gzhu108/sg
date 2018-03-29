#include "RequestGetSceneReactor.h"
#include "StreetGangSessionManager.h"
#include "WorldRequester.h"
#include "WorldClient.h"
#include "ConfigurationSingleton.h"
#include "ResponseGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestGetSceneReactor::RequestGetSceneReactor(Connection& connection, std::shared_ptr<RequestGetScene> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , StreetGangReactor(responder)
{
}

RequestGetSceneReactor::~RequestGetSceneReactor()
{
}

bool RequestGetSceneReactor::Process()
{
    if (mResponder == nullptr)
    {
        LOG("Invalid responder");
        return false;
    }

    auto configuration = ConfigurationSingleton::GetConfiguration();

    std::string protocol;
    std::string worldHost;
    uint16_t worldPort = 0;
    configuration->GetValue("Protocol", protocol);
    configuration->GetValue("WorldHost", worldHost);
    configuration->GetValue("WorldPort", worldPort);

    std::shared_ptr<Client> client = std::make_shared<WorldClient>(protocol, worldHost, worldPort);
    auto requester = std::make_shared<worldapi::WorldRequester>(*client->GetConnection());
    requester->GetWorld(InputMessage()->WorldId.cref(), reinterpret_cast<uintptr_t>(this));

    auto session = StreetGangSessionManager::GetInstance().GetSession(InputMessage()->WorldId.cref());
    if (session == nullptr)
    {
        return mResponder->SendErrorResponse(InputMessage()->TrackId.cref(), ResultCode::ErrorBadRequest, InputMessage()->Id.cref(), "Scene not found");
    }

    std::vector<Point<float>> items;
    session->GetItemsInRect(InputMessage()->Rect.cref(), items);

    if (items.size() == 0)
    {
        LOG("No item found Rect(%f, %f, %f, %f)",
            InputMessage()->Rect->mX,
            InputMessage()->Rect->mY,
            InputMessage()->Rect->mW,
            InputMessage()->Rect->mH);
    }

    return mResponder->SendGetSceneResponse(InputMessage()->TrackId.cref(), ResultCode::Success, session->Id.cref(), InputMessage()->Rect.cref(), items);
}

bool RequestGetSceneReactor::SendResponse(const streetgangapi::SessionId& sessionId, const std::vector<Point<float>>& items)
{
    return mResponder->SendGetSceneResponse(InputMessage()->TrackId.cref(), ResultCode::Success, sessionId, InputMessage()->Rect.cref(), items);
}
