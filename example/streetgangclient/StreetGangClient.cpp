#include "StreetGangClient.h"
#include "NetworkUtility.h"
#include "TaskManager.h"
#include "StreetGangApi.h"
#include "BinaryStreetGangRequester.h"
#include "StreetGangPBRequestEncoder.h"
#include "StreetGangClientMessageDecoder.h"
#include "StreetGangClientPBDecoder.h"
#include "ResponseErrorReactor.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


StreetGangClient::StreetGangClient(const std::string& protocol, const std::string& hostName, uint16_t port)
{
    std::shared_ptr<Dispatcher> messageDecoder;
    if (port == 7390)
    {
        messageDecoder = std::make_shared<StreetGangClientPBDecoder>();
        mStreetGangRequestEncoder = std::make_shared<StreetGangPBRequestEncoder>();
    }
    else
    {
        messageDecoder = std::make_shared<StreetGangClientMessageDecoder>();
        mStreetGangRequester = std::make_shared<BinaryStreetGangRequester>();
    }

    // Create client profile
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set(protocol);
    profile->Address.set(hostName);
    profile->Port.set(port);
    profile->Dispatcher.set(messageDecoder);

    profile->Dispatcher.cref()->MessageTimedOut.Connect([](const Dispatcher::TimedOutMessage& timedOutMessage)
    {
        auto responseError = std::make_shared<ResponseError>();
        responseError->Result.set((int32_t)ResultCode::ErrorTimeout);
        responseError->RequestId.set(std::static_pointer_cast<MessageBase>(timedOutMessage.mMessage)->Id.cref());

        auto responseErrorReactor = std::make_shared<ResponseErrorReactor>(timedOutMessage.mConnection, responseError, nullptr);
        responseErrorReactor->SetParent(timedOutMessage.mMessage);
        
        responseErrorReactor->Process();
    });

    auto connection = NetworkUtility::CreateConnection(profile);
    Initialize(connection, std::chrono::milliseconds(30));
}

StreetGangClient::~StreetGangClient()
{
}
