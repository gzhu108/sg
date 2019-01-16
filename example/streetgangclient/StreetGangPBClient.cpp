#include "PBRequestByebye.h"
#include "StreetGangPBClient.h"
#include "NetworkUtility.h"
#include "TaskManager.h"
#include "StreetGangPBClientDispatcher.h"
#include "Uuid.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangclient;


StreetGangPBClient::StreetGangPBClient(const std::string& protocol, const std::string& hostAddress, uint16_t port)
{
    auto dispatcher = std::make_shared<StreetGangPBClientDispatcher>();

    // Create client profile
    auto profile = std::make_shared<Profile>();
    profile->Protocol.set(protocol);
    profile->Address.set(hostAddress);
    profile->Port.set(port);
    profile->Dispatcher.set(dispatcher);

    auto connection = NetworkUtility::CreateConnection(profile);
    Initialize(connection, std::chrono::milliseconds(30));
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
