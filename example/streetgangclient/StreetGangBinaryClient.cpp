#include "StreetGangBinaryClient.h"
#include "SecureTcpSocket.h"
#include "TcpConnection.h"
#include "StreetGangBinaryClientDispatcher.h"
#include "BinaryResponseError.h"
#include "ResponseErrorReactor.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


StreetGangBinaryClient::StreetGangBinaryClient(const std::string& protocol, const std::string& hostName, uint16_t port)
{
    auto dispatcher = std::make_shared<StreetGangBinaryClientDispatcher>();

    // Create client profile
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set(protocol);
    profile->Address.set(hostName);
    profile->Port.set(port);
    profile->Dispatcher.set(dispatcher);

    profile->Dispatcher.cref()->MessageTimedOut.Connect([](const Dispatcher::TimedOutMessage& timedOutMessage)
    {
        auto responseError = std::make_shared<BinaryResponseError>();
        responseError->Result.set((int32_t)ResultCode::ErrorTimeout);
        responseError->RequestId.set(std::static_pointer_cast<MessageBase>(timedOutMessage.mMessage)->Id.cref());

        auto responseErrorReactor = std::make_shared<ResponseErrorReactor>(std::static_pointer_cast<Connection>(timedOutMessage.mConnection.shared_from_this()), responseError, nullptr);
        responseErrorReactor->SetOriginalMessage(timedOutMessage.mMessage);
        
        responseErrorReactor->Process();
    });

    auto socket = std::make_shared<SecureTcpSocket>();
    socket->ConfigureSslContext(SSLv23_client_method(), "Client.key", "Client.cer", SecureTcpSocket::VerifyPeer);
    //auto socket = std::make_shared<TcpSocket>();
    auto connection = std::make_shared<TcpConnection>(socket, profile);

    Initialize(connection, std::chrono::milliseconds(30));
}

StreetGangBinaryClient::~StreetGangBinaryClient()
{
}
