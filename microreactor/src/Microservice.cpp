#include "Microservice.h"
#include "NetworkUtility.h"

using namespace sg::microreactor;


Microservice::Microservice()
{
}

Microservice::Microservice(std::shared_ptr<Profile> profile)
    : mProfile(profile)
{
}

Microservice::Microservice(std::shared_ptr<Endpoint> endpoint)
    : mEndpoint(endpoint)
{
    if (mEndpoint != nullptr)
    {
        mProfile = endpoint->GetProfile();
    }
}

Microservice::~Microservice()
{
    Stop();
}

bool Microservice::Start()
{
    if (Initialize() && mEndpoint != nullptr)
    {
        if (mEndpoint->Start())
        {
            // Connection to the ConnectonMade signal
            mEndpoint->ConnectionMade.Connect(std::bind(&Microservice::OnConnectionMade, this, std::placeholders::_1), reinterpret_cast<uintptr_t>(this));
            return true;
        }
    }

    return false;
}

bool Microservice::Stop()
{
    if (mEndpoint != nullptr)
    {
        if (!mEndpoint->Stop())
        {
            return false;
        }

        // Disconnect the ConnectonMade signal
        mEndpoint->ConnectionMade.Disconnect(reinterpret_cast<uintptr_t>(this));
        mEndpoint = nullptr;
        return true;
    }

    return false;
}

bool Microservice::Initialize()
{
    if (mEndpoint != nullptr)
    {
        // Already initialized
        return true;
    }

    mEndpoint = NetworkUtility::CreateEndpoint(mProfile);
    if (mEndpoint == nullptr)
    {
        return false;
    }

    return true;
}

void Microservice::OnConnectionMade(const std::shared_ptr<Connection>& connection)
{
    std::string hostName = connection->Name.cref();
    std::string connectionName = std::string("[") + connection->GetPeerName() + "]:" + std::to_string(connection->GetPeerPort());

    connection->Closed.Connect([=]()
    {
        LOG("Disconnected %s -> %s", hostName.c_str(), connectionName.c_str());
    });

    LOG("Connection accepted %s -> %s", hostName.c_str(), connectionName.c_str());
}
