#include "PBRequestByebye.h"
#include "StreetGangPBClient.h"
#include "NetworkUtility.h"
#include "TaskManager.h"
#include "StreetGangPBClientDispatcher.h"
#include "Uuid.h"

using namespace microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


StreetGangPBClient::StreetGangPBClient(const std::string& protocol, const std::string& hostAddress, uint16_t port)
{
    auto dispatcher = std::make_shared<StreetGangPBClientDispatcher>();
    dispatcher->Protocol.set(protocol);
    dispatcher->Address.set(hostAddress);
    dispatcher->Port.set(port);

    auto connection = NetworkUtility::CreateConnection(dispatcher);
    Initialize(connection);
}

StreetGangPBClient::~StreetGangPBClient()
{
    if (mConnection != nullptr && !mConnection->IsClosed())
    {
        auto message = std::make_shared<PBRequestByebye>();
        message->TrackId.set(Uuid::GenerateUuid().ToString());

        std::stringstream stream;
        if (message->Encode(stream))
        {
            // Send byebye message
            mConnection->Send(stream);
        }
    }
}
