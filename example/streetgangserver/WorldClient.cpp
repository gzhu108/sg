#include "WorldClient.h"
#include "NetworkUtility.h"
#include "WorldClientDispatcher.h"
#include "worldapi/ResponseError.h"
#include "ResponseErrorReactor.h"

using namespace sg::microreactor;
using namespace worldapi;
using namespace streetgangserver;


WorldClient::WorldClient(const std::string& protocol, const std::string& hostName, uint16_t port)
{
    auto dispatcher = std::make_shared<WorldClientDispatcher>();

    // Create client profile
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set(protocol);
    profile->Address.set(hostName);
    profile->Port.set(port);
    profile->Dispatcher.set(dispatcher);

    profile->Dispatcher.cref()->MessageTimedOut.Connect([&](const Dispatcher::TimedOutMessage& timedOutMessage)
    {
        auto responseError = std::make_shared<ResponseError>();
        responseError->TrackId.set(timedOutMessage.mMessage->TrackId.cref());
        responseError->Result.set((int32_t)ResultCode::ErrorTimeout);
        responseError->RequestId.set(std::static_pointer_cast<WorldMessage>(timedOutMessage.mMessage)->Id.cref());
        responseError->ErrorMessage.set("WorldServer timeout");

        auto responseErrorReactor = std::make_shared<ResponseErrorReactor>(timedOutMessage.mConnection, responseError);
        responseErrorReactor->SetParent(timedOutMessage.mMessage);
        
        responseErrorReactor->Process();
    });

    auto connection = NetworkUtility::CreateConnection(profile);
    Initialize(connection, std::chrono::milliseconds(30));
}

WorldClient::~WorldClient()
{
}
